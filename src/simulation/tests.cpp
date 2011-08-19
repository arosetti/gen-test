#include "tests.h"
#include "general_tests.h"

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
        for (set<uint32>::iterator itr = m_faults_set.begin(); itr != m_faults_set.end(); ++itr)
            g_test->DeleteFault(*itr, 1);
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

string tests::get_dna() const 
{
    return "";
}; 

bool tests::IsEdited() const
{
    return true;
}

void tests::ExecuteTest(general_tests* g_test)
{
    if (!IsEdited())
    {
        // cout << "Test non eseguito perchè DNA uguale" << endl;
        return;
    }
    sim_test.execute(get_dna());
    sim_test.get_results(n_tests, detected);
    GetFaultsFile(g_test);
    UnsetEdited();
}

bool tests::GetFaultsFile(general_tests* g_test)
{
    string path = conf->simulator_dir;
    string fault  = "faults.txt";
    if (path.c_str()[path.length()] != '/')
        path += "/";
    path += fault;

    ifstream sim_fault_file;
    sim_fault_file.open(path.c_str(), ios::binary);

    if (!sim_fault_file.is_open())
    {
        printf("file: %s\n", path.c_str());
        perror("simulator faults.txt");
        return false;
    }

    EmptyFaults(g_test);

    sim_fault_file.seekg (0, ios::end);
    int length = sim_fault_file.tellg();
    sim_fault_file.seekg (0, ios::beg);

    char *buffer = new char[length + 2];

    sim_fault_file.read (buffer,length);
    sim_fault_file.close();

    buffer[length] = 'E';
    buffer[length+1] = ' ';

    int fault_index = 0;

    string p_buffer = strtok (buffer," "); // The
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
                cout << "Errore lettura faults.txt, numero faults superiore al massimo" << endl;
                //perror("simulator faults.txt");
                return false;
            }

            //cout << "insert fault " << fault_index << endl;
            InsertFault(fault_index);
            if (g_test)
                g_test->InsertFault(fault_index);            
        }

        p_buffer = strtok (NULL, "\n");
        p_buffer = strtok (NULL, " ");
        fault_index++;
    }

    remove(path.c_str());
    return true;
}
