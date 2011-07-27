#include"simulation.h"

simulation::simulation()
{
    //chdir((char *)conf->simulator_dir.c_str()); // getcwd(3)
    srand(time(NULL));
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

    if (chmod((char *) sim_bin.c_str(),775))
    {
        perror("chmod");
        return false;
    }

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

    chdir((char *)conf->simulator_dir.c_str());
    system(path.c_str());
    
    //strtok e arg-voilà-bella-come-una-string
    //int execl(path.c_str(), argvs);
    
    //controllo esecuzione terminata
    cout << endl;
    cout << read_output()    << endl;

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
    else
        cout << "printing simulator input file to " << get_input_file_path().c_str() << endl;

    int clocks = GetStrRowSize(dna);
    int inputs = GetStrColSize(dna);
    
    cout << "clocks: " << clocks << endl;
    cout << "inputs: " << inputs << endl;
    
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

    cout << "out : " << get_output_file_path().c_str();

    if (!sim_out_file.is_open())
    {
        perror("setup_output_file");
        exit(0);
    }
    else
        cout << endl << "retrieving simulator output file" << endl;

    sim_out_file.seekg (0, ios::end);
    length = sim_out_file.tellg();
    sim_out_file.seekg (0, ios::beg);

    buffer = new char[length];

    sim_out_file.read (buffer,length);
    sim_out_file.close();
    
    content = buffer;
    delete[] buffer;
    
    return content;
}
