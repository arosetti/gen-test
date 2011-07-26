#include"simulation.h"

simulation::simulation()
{

}

simulation::~simulation()
{

}

bool simulation::execute_simulation(string dna)
{
    string path = conf->simulator_dir;
    if (path.c_str()[path.length()] != "/")
        path += "/";
    path+= conf->simulator_bin;

    if(!file_exist(path))
    {
        cout << "can't find simulator binary." << endl;
        exit(0);
    }

    path += " ";
    path += conf->simulator_opts;
    system("");
}

