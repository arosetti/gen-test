#include"simulation.h"

simulation::simulation()
{

}

simulation::~simulation()
{

}

bool simulation::execute(string dna, uint32 id)
{
    int status, ret;
    pid_t pid;
    string args;

    setup_input_file(dna, id);

    if (!file_exists(get_bin_path(id).c_str()))
    {
        INFO("verbose", "* simulation: simulator binary does not exists.\n");
        exit(0);
    }

    pid = fork(); // con vfork gdb si blocca
    if (pid == 0)
    {
        int fd;
        ret = chdir((char *)get_sim_path(id).c_str());

        if((fd = open("/dev/null", O_RDWR | O_CREAT, 0777)) == -1)
        {
            perror("open");
            _exit(0);
        }

        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);

        ret = execle(get_bin_path(id).c_str(),
                     conf->get_string_config(CONFIG_SIMULATOR_BIN).c_str(),
                     "-N", "1", "-a",
                     conf->get_string_config(CONFIG_TEST_FILE_OUT).c_str()
                     , NULL, environ);

        perror("execle"); // qui non ci arriva mai ( si spera :D )
        _exit(0);
    }
    else if (pid == -1)
    {
        perror("fork");
        return false;
    }

    waitpid(pid, &status, 0);

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
    
    LOG("sim_events", "simulation", "clocks: %d", clocks);
    LOG("sim_events", "simulation", "inputs: %d", inputs);

    
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
        LOG("sim_events", "simulation", "parsing error... ret is %d\n", ret);

    LOG("sim_events", "simulation", "#total_faults       = %d\n", n_total_faults);
    LOG("sim_events", "simulation", "#faults_detected    = %d\n", n_faults);

    remove(get_output_file_path(id).c_str());
    remove(get_input_file_path(id).c_str());

    delete[] str;
}

void simulation::get_results(uint32 id, uint32& n_total_faults, uint32& n_faults, float &propagation_f)
{
    int ret;
    char *str = read_file(get_output_file_path(id).c_str());
    char *str_s = strstr (str,"0,");

    n_total_faults = 0;
    n_faults = 0;
    
    ret = sscanf(str_s, "0,%d.0\n1,%d.0\n2,%f\n", (int*)&n_total_faults, (int*)&n_faults,(float*)&propagation_f);
    
    if (ret != 3)
        LOG("sim_events", "simulation", "parsing error... ret is %d\n", ret);

    LOG("sim_events", "simulation", "#total_faults       = %d\n", n_total_faults);
    LOG("sim_events", "simulation", "#faults_detected    = %d\n", n_faults);
    LOG("sim_events", "simulation", "#propagation_factor = %f\n", propagation_f);

    remove(get_output_file_path(id).c_str());
    remove(get_input_file_path(id).c_str());

    delete[] str;
}
