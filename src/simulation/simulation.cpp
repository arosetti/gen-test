#include"simulation.h"

simulation::simulation()
{

}

simulation::~simulation()
{

}

bool simulation::execute(string dna, uint32 id)
{
    int status, ret, f_ret;

    setup_input_file(dna, id);

    if (!file_exists(get_bin_path(id).c_str()))
    {
        INFO("verbose", "* simulation: simulator binary does not exists.\n");
        exit(0);
    }

    f_ret = fork(); //vfork();
    if (f_ret == 0)
    {   
        ret = chdir((char *)get_sim_path(id).c_str());
        ret = exec_command("./%s %s%s > /dev/null 2>&1",
                          conf->get_string_config(CONFIG_SIMULATOR_BIN).c_str(),
                          conf->get_string_config(CONFIG_SIMULATOR_ARGS).c_str(),
                          conf->get_string_config(CONFIG_TEST_FILE_OUT).c_str());
        _exit(0);
    }
    else if (f_ret == -1)
    {
        perror("fork");
        return false;
    }

    waitpid(f_ret, &status, 0);

    return true;
}

bool simulation::setup_input_file(string dna, uint32 id)
{
    fstream sim_file;

    sim_file.open(get_input_file_path(id).c_str(),  fstream::out);

    if (!sim_file.is_open())
    {
        perror("input_file");
        exit(0);
    }

    int clocks = GetStrRowSize(dna);
    int inputs = GetStrColSize(dna);
    
    if (conf->get_bool_config(CONFIG_DEBUG) && conf->get_bool_config(CONFIG_LOG_SIMULATION))
    {
        cout << "clocks: " << clocks << endl;
        cout << "inputs: " << inputs << endl;
    }
    
    stringstream out;
    out << clocks;

    sim_file << out.str();
    sim_file << ";" << endl;

    for (uint32 i = 0 ; i < inputs; i++)
    {
        for (uint32 j = 0 ; j < clocks; j++)
        {
            sim_file << dna.c_str()[i * (clocks +1) + j];
            if (j != (clocks - 1))
                sim_file << ",";
        }
        sim_file << ";" << endl;
    }

    sim_file.close();
    return true;
}

void simulation::get_results(uint32 id, uint32& n_total_faults, uint32& n_faults)
{
    int ret;
    char *str = read_file(get_output_file_path(id).c_str());
    char *str_s = strstr (str,"0,");

    n_total_faults = 0;
    n_faults = 0;
    
    ret = sscanf(str_s, "0,%d.0\n1,%d.0\n", (int*)&n_total_faults, (int*)&n_faults);
    if (ret != 2)
        cout << "parsing error... ret " << ret << endl;

    if (conf->get_bool_config(CONFIG_LOG_SIMULATION)) // TODO sistemare log
    {
        cout << "#total_faults " << n_total_faults << endl;
        cout << "#faults_detected " << n_faults << endl;
    }

    remove(get_output_file_path(id).c_str());
    remove(get_input_file_path(id).c_str());

    delete[] str;
}
