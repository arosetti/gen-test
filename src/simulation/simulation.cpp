#include"simulation.h"

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

simulation::simulation()
{
    init_env();

}

simulation::~simulation()
{

}

void simulation::rebuild()
{

}

string simulation::get_bin_path()
{
    string path = conf->simulator_dir; 

    if (path.c_str()[path.length()] != '/') // funzione add slash
        path += "/";
    path += conf->simulator_bin;
    
    return path;
}

string simulation::get_patch_path()
{
    string path = conf->simulator_dir;

    if (path.c_str()[path.length()] != '/')
        path += "/";
    path += conf->simulator_patch;
    
    return path;
}

string simulation::get_input_file_path()
{
    string path = conf->simulator_dir;

    if (path.c_str()[path.length()] != '/')
        path += "/";
    path += conf->test_file_in;
    
    return path;
}

string simulation::get_output_file_path()
{
    string path = conf->simulator_dir;

    if (path.c_str()[path.length()] != '/')
        path += "/";
    path += conf->test_file_out;
    
    return path;
}

bool simulation::init_env()
{
    string sim_bin = get_bin_path();

    /*if (chmod((char *) sim_bin.c_str(),00775))
    {
        perror("chmod");
        return false;
    }*/

    return true;
}

bool simulation::execute(string dna)
{
    string path = get_bin_path();
    char *buffer = new char[1024];

    if (!file_exists(path))
    {
        cout << "simulator binary does note exists." << endl;
        exit(0);
    }

    path += " ";
    path += conf->simulator_args;
    path += conf->test_file_out;
    path += " > /dev/null 2>&1";

    setup_input_file(dna);

    getcwd(buffer,1024);
    chdir((char *)conf->simulator_dir.c_str());
    system(path.c_str()); //int execl(path.c_str(), argvs);
    chdir(buffer);

    delete[] buffer;
    return true;
}

bool simulation::setup_input_file(string dna)
{
    fstream sim_file;
    
    sim_file.open(get_input_file_path().c_str(),  fstream::out);

    if (!sim_file.is_open())
    {
        perror("setup_input_file");
        exit(0);
    }

    int clocks = GetStrRowSize(dna);
    int inputs = GetStrColSize(dna);
    
    if (conf->debug && conf->print_simulation)
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

string simulation::read_output()
{
    uint32 length;
    ifstream sim_out_file;
    string content;
    char *buffer = NULL;

    sim_out_file.open (get_output_file_path().c_str(), ios::binary );

    if (conf->debug && conf->print_simulation)
    {
        cout << "out : " << get_output_file_path().c_str() << endl;
    }
    
    if (!sim_out_file.is_open())
    {
        perror("setup_output_file");
        exit(0);
    }

    sim_out_file.seekg (0, ios::end);
    length = sim_out_file.tellg();
    sim_out_file.seekg (0, ios::beg);

    buffer = new char[length + 1];

    sim_out_file.read (buffer,length);
    sim_out_file.close();
    remove(get_input_file_path().c_str()); // DEBUG mi assicuro che il vecchio file di ingresso venga rimosso
    buffer[length] = 0;
    content = buffer;
    delete[] buffer;
    
    return content;
}

void simulation::get_results(uint32 *n_total_faults, uint32 *n_faults)
{
    int ret;
    string str = read_output();
    char *str_s = strstr ((char *)str.c_str(),"0,");

    *n_total_faults = 0; // se si rimuove questo nn funziona nulla (forse ora è ok ma non ci conterei)
    *n_faults = 0;
    
    ret = sscanf(str_s, "0,%d.0\n1,%d.0\n", (int*)n_total_faults, (int*)n_faults);
    if (ret != 2)
        cout << "parsing error... ret " << ret << endl;

    if (conf->debug && conf->print_simulation)
    {
        cout << "#total_faults " << *n_total_faults << endl;
        cout << "#faults_detected " << *n_faults << endl;
    }
    if (*n_total_faults > 1000)
        exit(0);

    remove(get_output_file_path().c_str());
}
