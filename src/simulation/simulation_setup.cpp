#include "simulation_setup.h"

string get_sim_path()
{
    string path;
    char *buffer = new char[1024];

    getcwd(buffer,1024);
   
    if(conf->simulator_dir.c_str()[0] != '/')
    {
        path = buffer;
        addslash(path);
    }
    path += conf->simulator_dir;

    addslash(path);

    return path;
}

string get_bin_path()
{
    string path = get_sim_path();
    path += conf->simulator_bin;
    
    return path;
}

string get_outputnet_path()
{
    string path = get_sim_path();
    path += "output.net";
    
    return path;
}

string get_faults_path()
{
    string path = get_sim_path();
    path += "faults.txt";
    
    return path;
}


string get_patch_path()
{
    string path = conf->simulator_patch;
    return path;
}

string get_input_file_path()
{
    string path = get_sim_path();
    path += conf->test_file_in;

    return path;
}

string get_output_file_path()
{
    string path = get_sim_path();
    path += conf->test_file_out;

    return path;
}

bool init_env()
{
    string sim_bin = get_bin_path();
    string thread_dir, bin_link, outputnet_link;
    int ret;

    if (conf->debug && conf->verbose)
        cout << "* simulation: init environment" << endl;

    remove(get_faults_path().c_str());
    
    for(int i = 0 ; i < conf->thread_slots ; i++ )
    {
        stringstream out;
        out << i;
        thread_dir = get_sim_path();
        thread_dir += conf->thread_prefix;
        thread_dir += out.str();
        addslash(thread_dir);
        mkdir(thread_dir.c_str(), 0777);
        
        bin_link = thread_dir;
        bin_link += conf->simulator_bin;
        outputnet_link = thread_dir;
        outputnet_link += "output.net";        
        
        ret = symlink(get_outputnet_path().c_str(), outputnet_link.c_str());
        ret = symlink(get_bin_path().c_str(), bin_link.c_str());
    }
    
    /*if (chmod((char *) sim_bin.c_str(),00775))
    {
        perror("chmod");
        return false;
    }*/

    return true;
}

uint32 read_n_inputs()
{
    string path = conf->simulator_dir;
    string net  = "output.net";
    if (path.c_str()[path.length()] != '/')
        path += "/";
    path += net;

    ifstream sim_net_file;
    char *buffer = NULL, *p_buffer = NULL;
    uint32 n_inputs = 0;
    int length = 0;

    sim_net_file.open(path.c_str(), ios::binary);

    if (!sim_net_file.is_open())
    {
        printf("file: %s\n", path.c_str());
        perror("simulator output.net");
        return 0;
    }

    sim_net_file.seekg (0, ios::end);
    length = sim_net_file.tellg();
    sim_net_file.seekg (0, ios::beg);

    buffer = new char[length + 1];

    sim_net_file.read (buffer,length);
    sim_net_file.close();
    buffer[length] = 0;

    p_buffer = strtok (buffer,"\n");
    while (p_buffer != NULL)
    {
        if (strncmp(p_buffer + (strchr(p_buffer,' ') - p_buffer + 1), "ibuf ",4) == 0)
            n_inputs++;
        else
            break;

        p_buffer = strtok (NULL, "\n");
        if(p_buffer == NULL)
        {
           n_inputs = 0;
           break;
        }
    }

    delete[] buffer;
    
    return n_inputs;

}
