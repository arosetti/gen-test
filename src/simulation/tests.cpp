#include "tests.h"
#include "general_tests.h"
#include "../ga/population.h"

/// Corpo del thread di simulazione

void *SimulationThread(void *arg)
{
    thread_params* t_param = (thread_params*) arg;

    remove(get_faults_path(t_param->sim_id).c_str());

    while (individual* ind = t_param->pop->get_next_ind())
    {        
        if (ind->ExecuteTest(t_param->sim_id, t_param->g_test))
            usleep(50); // Per evitare che si accavallano i log
        t_param->pop->inc_barlink();
    }

    t_param->pop->dec_threads();
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

uint32 tests::GetTestNumber()
{
    return n_tests;
}

uint32 tests::GetDetectedNumber()
{
    return detected;
}

void tests::EmptyFaults(general_tests* g_test)
{
    if (g_test)
    {
        for (set<uint32>::iterator itr = m_faults_set.begin(); itr != m_faults_set.end(); ++itr)
        {
            g_test->DeleteFault(*itr, 1);
        }
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
    if (is_tested())
    {
        //if (conf->debug && conf->verbose)
        //    cout << "skipped" << endl;
        return false;
    }

    //if (conf->debug && conf->verbose)
    //    cout << "testing" << endl;

    int tried = 0;
    while (!is_tested() && tried < (conf->max_retest + 1))
    {
        try
        {
            sim_test.execute(get_dna(), sim_id);
            sim_test.get_results(sim_id, n_tests, detected);
            GetFaultsFile(sim_id, g_test);
            test();
        }
        catch (char const* str)
        {            
            tried++;
            if (conf->debug && conf->verbose)
            {
                cout << "caught exception: " << str << endl;
                if (tried < (conf->max_retest + 1))
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

    sim_fault_file.seekg (0, ios::end);
    int length = sim_fault_file.tellg();
    sim_fault_file.seekg (0, ios::beg);

    if (!length)
    {
        throw "fault.txt is empty";
    }

    char *buffer = new char[length + 2];

    sim_fault_file.read (buffer,length);
    sim_fault_file.close();

    remove(path.c_str());

    buffer[length] = 'E';
    buffer[length+1] = ' ';

    int fault_index = 0;

    string p_buffer = strtok (buffer," ");

    if (g_test)
        g_test->getlock_gen_test();  // Lock general tests

    while (p_buffer != "E")
    {
        p_buffer = strtok (NULL, " "); // fault
        p_buffer = strtok (NULL, " ");

        if (p_buffer != "in") //!!!!
            p_buffer = strtok (NULL, " "); //in

        p_buffer = strtok (NULL, " "); // component
        p_buffer = strtok (NULL, " "); // number
        p_buffer = strtok (NULL, " "); // has
        p_buffer = strtok (NULL, " ");

        if (p_buffer == "been") // fault trovato
        {
            if (fault_index > n_tests)
            {
                if (g_test)
                    g_test->releaselock_gen_test();  // Release Lock general tests
                delete[] buffer;

                throw "error reading faults.txt, max number of faults";
                return false;
            }

            InsertFault(fault_index);
            if (g_test)
                g_test->InsertFault(fault_index);
        }

        p_buffer = strtok (NULL, "\n");
        p_buffer = strtok (NULL, " ");
        fault_index++;
    }

    if (g_test)
        g_test->releaselock_gen_test();  // Release Lock general tests
    delete[] buffer;

    return true;
}
