#ifndef SIMULATION_H
#define SIMULATION_H

#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>

#include "../common/utility.h"
#include "../config.h"
#include "../bitmatrix/bitmatrix.h"

using namespace std;

extern config *conf;

class simulation
{
    bool setup_input_file(string);
    
    string get_bin_path();
    string get_patch_path();
    string get_input_file_path();

    public:
    simulation();
    ~simulation();
    
    void rebuild();
    void patch();
    bool check_env();
    bool execute(string);
};

#endif
