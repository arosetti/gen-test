#include"simulation.h"

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

    if (path.c_str()[path.length()] != '/')
        path += "/";
    path+= conf->simulator_bin;
    
    return path;
}

string simulation::get_patch_path()
{
    string path = conf->simulator_dir;

    if (path.c_str()[path.length()] != '/')
        path += "/";
    path+= conf->simulator_patch;
    
    return path;
}

string simulation::get_input_file_path()
{
    string path = conf->simulator_dir;

    if (path.c_str()[path.length()] != '/')
        path += "/";
    path+= conf->test_file_in;
    
    return path;
}

string simulation::get_output_file_path()
{
    string path = conf->simulator_dir;

    if (path.c_str()[path.length()] != '/')
        path += "/";
    path+= conf->test_file_out;
    
    return path;
}

bool simulation::init_env()
{
    string sim_bin = get_bin_path();

    /*if (chmod((char *) sim_bin.c_str(),775))
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
    //char *args[];

    if (!file_exists(path))
    {
        cout << "can't find simulator binary." << endl; // perror
        exit(0);
    }

    path += " ";
    path += conf->simulator_args;
    path += " > /dev/null 2>&1";

    setup_input_file(dna);

    getcwd(buffer,1024);
    chdir((char *)conf->simulator_dir.c_str());
    system(path.c_str());
    chdir(buffer);
    
    //strtok e arg-voilà-bella-come-una-string
    //int execl(path.c_str(), argvs);

    //controllo esecuzione terminata
    if (conf->debug && conf->print_simulation)
    {
        cout << endl;
        cout << read_output()    << endl;
    }
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

    buffer = new char[length];

    sim_out_file.read (buffer,length);
    sim_out_file.close();
    remove(get_output_file_path().c_str()); // DEBUG mi assicuro che il vecchio file venga rimosso

    content = buffer;
    delete[] buffer;
    
    return content;
}

float simulation::get_results()
{
    int ret, n_total_faults = 0 , n_faults = 0;
    float val = 0;
    string str = read_output();

    // orribile cosa, da fare per bene una regex
    ret = sscanf(str.c_str(), "#MOBIUS_SIM_ASCII_FILE,MOBIUS_VER=Version 2.3,FILE_VER=1,EXPERIMENTS=0\n#FORMAT: (PV ID Number),(PV Value)\n0,%d.0\n1,%d.0", &n_total_faults, &n_faults);

    if (ret != 2)
        cout << "parsing error... ret " << ret << endl;
    else
        val = (n_faults * 100) / n_total_faults;  // check div by 0

    if (conf->debug && conf->print_simulation)
    {
        cout << "#total_faults " << n_total_faults << endl;
        cout << "#faults_detected " << n_faults << endl;
        cout << "fitness " << val << endl;
    }

    return val;
}
