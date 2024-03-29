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
#include "../common/logger/logger.h"
#include "../bitmatrix/bitmatrix.h"

using namespace std;

string get_sim_path(int);
string get_bin_path(int);
string get_outputnet_path(int);
string get_faults_path(int);
string get_input_file_path(int);
string get_output_file_path(int);
string get_log_file_path(uint32);

void clean_env();
void init_env();

uint32 read_n_inputs();

#endif
