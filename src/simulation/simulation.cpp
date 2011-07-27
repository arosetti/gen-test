#include"simulation.h"

simulation::simulation()
{

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

bool simulation::check_env()
{
    return true;
}

bool simulation::execute(string dna)
{
    string path = get_bin_path();
    //char *args[];

    if (!file_exists(path))
    {
        cout << "can't find simulator binary." << endl; // perror
        exit(0);
    }

    path += " ";
    path += conf->simulator_args;
    
    setup_input_file(dna);

    system(path.c_str());
    
    //strtok e arg-voilà-bella-come-una-string
    //int execl(path.c_str(), argvs);
    
    //controllo esecuzione terminata
    
    return true;
}

bool simulation::setup_input_file(string dna)
{
    fstream sim_file;
    
    sim_file.open(get_input_file_path().c_str(),  fstream::out);

    if (!sim_file.is_open())
    {
        perror("setup_input_file: ");
        exit(0);
    }

    int clocks = GetStrColSize(dna);
    int inputs = GetStrRowSize(dna);

    stringstream out;
    out << clocks;

    sim_file << out.str();
    sim_file << ";" << endl;

    for (uint32 i = 0 ; i < inputs; i++)
    {
        for (uint32 j = 0 ; j < clocks; j++)
        {
            sim_file << dna.c_str()[j * (clocks +1) + i];
            sim_file << "; ";
        }
        sim_file << endl;
    }

    sim_file.close();
    return true;
}
