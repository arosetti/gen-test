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
    //char *args[];

    if (path.c_str()[path.length()] != '/')
        path += "/";
    path+= conf->simulator_bin;

    if (!file_exists(path))
    {
        cout << "can't find simulator binary." << endl;
        exit(0);
    }

    path += " ";
    path += conf->simulator_args;
    
    setup_simulator_input_file(dna);

    system(path.c_str());
    
    //strtok e arg-voilà
    //int execl(path.c_str(), argvs);
    
    //controllo esecuzione terminata
    
    return true;
}

void simulation::setup_simulator_input_file(string dna)
{
    string input_format;
    int clocks = GetStrColSize(dna);
    int inputs = GetStrRowSize(dna);

    stringstream out;
    out << clocks;

    input_format += out.str();
    input_format += ";\n";

    for(uint32 i = 0 ; i < inputs; i++)
    {
        for(uint32 j = 0 ; j < clocks; j++)
        {
            input_format += dna.c_str()[j * (clocks +1) + i];
            input_format += "; ";
        }
        input_format += "\n";
    }
}
