#include "simulation_utility.h"

extern config* conf;

string get_thread_subdir(uint32 id)
{
    stringstream out;
    out << id;
    string subdir = conf->get_string_config(CONFIG_THREAD_PREFIX);
    subdir += out.str();
    addslash(subdir);

    return subdir;
}

string get_sim_path(int id)
{
    string path;
    char *buffer = new char[1024];

    if (conf->get_string_config(CONFIG_SIMULATOR_PATH).c_str()[0] != '/')
    {
        path = conf->get_string_config(CONFIG_MAIN_PATH);
        addslash(path);
    }
    path += conf->get_string_config(CONFIG_SIMULATOR_PATH);

    addslash(path);

    if (id>=0)
        path += get_thread_subdir(id);

    delete buffer;
    return path;
}

string get_bin_path(int id)
{
    string path = get_sim_path(id);
    path += conf->get_string_config(CONFIG_SIMULATOR_BIN);

    return path;
}

string get_outputnet_path(int id)
{
    string path = get_sim_path(id);
    path += "output.net";

    return path;
}

string get_faults_path(int id)
{
    string path = get_sim_path(id);
    path += "faults.txt";

    return path;
}

string get_input_file_path(int id)
{
    string path = get_sim_path(id);
    path += conf->get_string_config(CONFIG_TEST_FILE_IN);

    return path;
}

string get_output_file_path(int id)
{
    string path = get_sim_path(id);
    path += conf->get_string_config(CONFIG_TEST_FILE_OUT);

    return path;
}

string get_log_file_path(uint32 gen)
{
    stringstream str;
    str << conf->get_string_config(CONFIG_MAIN_PATH) << "/" << conf->get_string_config(CONFIG_LOG_PATH) << "/" << "generaton" << gen << ".log";

    return str.str();
}

void clean_env()
{
    INFO("verbose", "* simulation: cleaning environment\n");

    remove(get_faults_path(-1).c_str());
    remove(get_input_file_path(-1).c_str());
    remove(get_output_file_path(-1).c_str());

    for(int i = 0 ; i < conf->get_int_config(CONFIG_THREAD_SLOTS); i++)
    {
        remove(get_faults_path(i).c_str());
        remove(get_input_file_path(i).c_str());
        remove(get_output_file_path(i).c_str());
        unlink(get_outputnet_path(i).c_str());
        unlink(get_bin_path(i).c_str());
        rmdir(get_sim_path(i).c_str());
    }
}

void init_env()
{
    int ret;

    clean_env();
    INFO("verbose","* simulation: initing \"%s\" using %d thread(s)\n", 
          conf->get_string_config(CONFIG_SIMULATOR_PATH).c_str(),
          conf->get_int_config(CONFIG_THREAD_SLOTS));

    for(int i = 0 ; i < conf->get_int_config(CONFIG_THREAD_SLOTS) ; i++ )
    {
        mkdir(get_sim_path(i).c_str(), 0777);
        ret = symlink(get_outputnet_path(-1).c_str(),
                      get_outputnet_path(i).c_str());
        if (ret)
            perror("symlink");
        ret = symlink(get_bin_path(-1).c_str(), get_bin_path(i).c_str());
        if (ret)
            perror("symlink");
    }
    
    if (chmod((char *) get_bin_path(-1).c_str(),0777))
    {
        perror("chmod");
    }
}

uint32 read_n_inputs()
{
    char *buffer = NULL, *p_buffer = NULL;
    uint32 n_inputs = 0;

    if ((buffer = read_file(get_outputnet_path(-1).c_str())) == 0)
        return 0;

    p_buffer = strtok(buffer,"\n");
    while (p_buffer != NULL)
    {
        if (strncmp(strchr(p_buffer, ' ') + 1, "ibuf ",4) == 0)
            n_inputs++;
        else
            break;

        p_buffer = strtok(NULL, "\n");
        if(p_buffer == NULL)
        {
           n_inputs = 0;
           break;
        }
    }

    delete[] buffer;
    return n_inputs;
}
