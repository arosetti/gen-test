#include "tests.h"
#include "general_tests.h"
#include "../ga/population.h"

/// Corpo del thread di simulazione

void *SimulationThread(void *arg)
{
    thread_params* t_param = (thread_params*) arg;

    sigset_t mask;
    sigfillset(&mask);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    if (conf->get_int_config(CONFIG_FITNESS_TYPE) == FAULT_RATE_MIN_LENGTH_WITH_NEIGHBOURS ||
        conf->get_int_config(CONFIG_FITNESS_TYPE) == FAULT_RATE_MIN_LENGTH_MAX_PROPAGATION_WITH_NEIGHBOURS)
        remove(get_faults_path(t_param->sim_id).c_str());

    while (individual* ind = t_param->pop->get_next_individual())
    {
        if (!ind->is_tested())
        {
            ind->ExecuteTest(t_param->sim_id, t_param->g_test);
            t_param->pop->inc_barlink();
        }
    }

    t_param->pop->dec_threads();

    if (t_param)
        delete t_param;

    pthread_exit(NULL);
}


/// Classe TESTS

tests::tests()
{
    n_tests = 0;
    detected = 0;
    m_faults_set.clear();
}

tests::tests(const tests &m_test)
{
    n_tests = m_test.n_tests;
    detected = m_test.detected;
    m_faults_set = m_test.m_faults_set;
    sim_test = m_test.sim_test;
}

tests::~tests()
{

}

uint32 tests::GetTestNumber() const
{
    return n_tests;
}

uint32 tests::GetDetectedNumber() const
{
    return detected;
}

float tests::GetFaultCoverage() const
{
    if (n_tests)
        return (float)(detected) / (float)(n_tests);
    else
        return 0.0f;
}

void tests::EmptyFaults(general_tests* g_test)
{    
    if (g_test)
    {
        g_test->getlock_gen_test(); 
        for (set<uint32>::iterator itr = m_faults_set.begin(); itr != m_faults_set.end(); ++itr)
        {
            g_test->DeleteFault(*itr, 1);
        }
        g_test->releaselock_gen_test();
    }    
    m_faults_set.clear();
}

uint32 tests::GetFaultsSize()
{
    return m_faults_set.size();
}

set<uint32> const& tests::GetFaultsList() const
{
    return m_faults_set;
}

bool tests::DeleteFault(uint32 fault)
{
    set<uint32>::iterator itr = m_faults_set.find(fault);
    if (itr == m_faults_set.end())
        return false;

    m_faults_set.erase(itr);

    return true;
}

bool tests::InsertFault(uint32 fault)
{
    set<uint32>::iterator itr = m_faults_set.find(fault);
    if (itr != m_faults_set.end())
        return false;

    m_faults_set.insert(fault);

    return true;
}

bool tests::FindFault(uint32 fault)
{
    set<uint32>::iterator itr = m_faults_set.find(fault);
    if (itr == m_faults_set.end())
        return false;

    return true;
}

bool tests::ExecuteTest(uint32 sim_id, general_tests* g_test)
{
    int tried = 0;
    while (!is_tested() && tried < (conf->get_int_config(CONFIG_MAX_RETEST) + 1))
    {
        try
        {
            sim_test.execute(get_dna(), sim_id);
            sim_test.get_results(sim_id, n_tests, detected, propagation_factor);
            if (conf->get_int_config(CONFIG_FITNESS_TYPE) == FAULT_RATE_MIN_LENGTH_WITH_NEIGHBOURS ||
                conf->get_int_config(CONFIG_FITNESS_TYPE) == FAULT_RATE_MIN_LENGTH_MAX_PROPAGATION_WITH_NEIGHBOURS)
                GetFaultsFile(sim_id, g_test);
            test();
        }
        catch (char const* str)
        {
            tried++;
            if (conf->get_bool_config(CONFIG_DEBUG) && conf->get_bool_config(CONFIG_VERBOSE))
            {
                cout << "caught exception: " << str << endl;
                if (tried < (conf->get_int_config(CONFIG_MAX_RETEST) + 1))
                    cout << "retesting try " << tried << endl;
            }
        }
    }
    return true;
}

bool tests::GetFaultsFile(uint32 sim_id, general_tests* g_test)
{
    string path = get_faults_path(sim_id);
    ifstream sim_fault_file;
    sim_fault_file.open(path.c_str(), ios::binary);

    if (!sim_fault_file.is_open())
    {
        throw "can't open faults.txt";
        return false;
    }

    EmptyFaults(g_test);

    int fault_index = 0;

    // Parse the data file
    while (!sim_fault_file.eof())
    {
        char buf[200] = {0};
        int word = 0;
        string Field;

        sim_fault_file.getline(buf, sizeof(buf));

        istringstream istr(string(buf), ios_base::out);

        while (istr >> Field)
        {
            word++;
            switch (word)
            {
                case 1: // The
                case 2: // fault
                break;
                case 3: // in or !!!!!!
                    if (Field == "in")
                        word++;
                break;
                case 4: // in
                case 5: // component
                case 6: // number
                case 7: // has
                break;
                case 8: // been or not
                    if (Field == "been")
                    {
                        if (fault_index > n_tests)
                        {
                            throw "error reading faults.txt, max number of faults";
                            return false;
                        }

                        InsertFault(fault_index);
                        if (g_test)
                        {      
                            g_test->getlock_gen_test(); 
                            g_test->InsertFault(fault_index);
                            g_test->releaselock_gen_test();
                        }
                    }
                    word = 10;
                break;
                default:
                    word = 10;
                break;
            }

            if (word == 10)
                break;
        }
        fault_index++;
    }

    sim_fault_file.close();
    remove(path.c_str());

    return true;
}

float tests::calculate_neighbours_fault_factor(general_tests* g_test)
{   
    float factor = 0.0f;
    if (g_test)
        for (set<uint32>::iterator itr = m_faults_set.begin(); itr != m_faults_set.end(); ++itr)
        {         
            factor += 1.0f/float(g_test->FindFault(*itr));
        }
    return factor;
}
