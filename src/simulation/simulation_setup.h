#ifndef SIMULATION_SETUP_H
#define SIMULATION_SETUP_H

#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
//#include <sys/types.h>
//#include <regex.h>

#include "../config.h"
#include "../bitmatrix/bitmatrix.h"

extern string dir_path;

string get_sim_path(int);
string get_bin_path(int);
string get_outputnet_path(int);
string get_faults_path(int);
string get_patch_path(int);
string get_input_file_path(int);
string get_output_file_path(int);

bool init_env();
uint32 read_n_inputs();

#endif
