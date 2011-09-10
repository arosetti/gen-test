#include "general_tests.h"

general_tests::general_tests()
{    
    pthread_mutex_init(&mutex_gen_test, NULL);
}

general_tests::~general_tests()
{

}

void general_tests::EmptyFaults()
{
    m_faults_map.clear();
}

uint32 general_tests::GetFaultsSize()
{
    return m_faults_map.size();
}

FaultsMap const& general_tests::GetFaultsMap() const
{
    return m_faults_map;
}

bool general_tests::DeleteFault(uint32 fault, uint32 numb)
{
    FaultsMap::iterator itr = m_faults_map.find(fault);
    if (itr == m_faults_map.end())
        return false;

    if (m_faults_map[fault] <= numb || !numb)
        m_faults_map.erase(fault);
    else
        m_faults_map[fault] -= numb;

    return true;
}

bool general_tests::InsertFault(uint32 fault, uint32 numb)
{
    FaultsMap::iterator itr = m_faults_map.find(fault);
    if (itr != m_faults_map.end())
        m_faults_map[fault] += numb;
    else
        m_faults_map[fault] = numb;

    return true;
}

uint32 general_tests::FindFault(uint32 fault)
{
    FaultsMap::iterator itr = m_faults_map.find(fault);
    if (itr == m_faults_map.end())
        return 0;

    return m_faults_map[fault];
}

// Deprecata (ma può servire in futuro)
bool general_tests::LoadFile(individual_map* pool)
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

    sim_fault_file.seekg (0, ios::end);
    int length = sim_fault_file.tellg();
    sim_fault_file.seekg (0, ios::beg);

    char *buffer = new char[length + 2];

    sim_fault_file.read (buffer,length);
    sim_fault_file.close();

    buffer[length] = 'E';
    buffer[length+1] = ' ';

    int test_index = 0;
    int fault_index = 0;

    int num_fault_block = 0;
    int fault_number = -1;

    string p_buffer = strtok (buffer," "); // The
    while (p_buffer != "E")
    {
        p_buffer = strtok (NULL, " "); // fault
        p_buffer = strtok (NULL, " ");

        bool new_block = false;
        if (p_buffer != "in") //!!!!
        {
            string block = "";
            for (int i = 0; i < p_buffer.size(); i++)
                block += "!";

            if (p_buffer == block)
                new_block = true;

            p_buffer = strtok (NULL, " "); //in
        }
        else
            new_block = true;


        p_buffer = strtok (NULL, " "); // component
        p_buffer = strtok (NULL, " "); // number

        int temp_fault_number = atoi(p_buffer.c_str());
        if (fault_number == -1)
        {            
            fault_number = temp_fault_number;
        }
        else if (fault_number == temp_fault_number && new_block)
        {
            fault_index = 0;
            test_index++;
        }
        
        p_buffer = strtok (NULL, " "); // has

        p_buffer = strtok (NULL, " ");
        if (p_buffer == "been") // fault trovato
        {
            InsertFault(fault_index);
            individual_map::iterator itr = pool->find(test_index);
            if (itr != pool->end())
                (*itr).second->InsertFault(fault_index); 
            //cout << "ADD: " << test_index << " " << fault_index <<endl;
        }

        p_buffer = strtok (NULL, "\n");
        p_buffer = strtok (NULL, " ");
        fault_index++;
    }

    remove(path.c_str());
    return true;
}
