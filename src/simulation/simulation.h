#ifndef SIMULATION_H
#define SIMULATION_H

#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
//#include <sys/types.h>
//#include <regex.h>

#include "../config.h"
#include "../bitmatrix/bitmatrix.h"

using namespace std;

extern config *conf;

uint32 read_n_inputs();

class simulation
{
    uint32 n_total_faults;
    uint32 n_fautls;

    bool setup_input_file(string);
    string read_output();

    string get_sim_path();
    string get_bin_path();
    string get_outputnet_path();
    string get_faults_path();
    string get_patch_path();
    string get_input_file_path();
    string get_output_file_path();

    public:
    simulation();
    ~simulation();
    
    void get_results(uint32&, uint32&);

    void rebuild();
    void patch();
    bool init_env();
    bool execute(string);
};

#endif
