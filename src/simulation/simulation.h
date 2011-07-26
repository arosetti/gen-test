#ifndef SIMULATION_H
#define SIMULATION_H

#include <cstdlib>
#include <string>
#include <sstream>

#include "../common/utility.h"
#include "../config.h"
#include "../bitmatrix/bitmatrix.h"

using namespace std;

extern config *conf;

class simulation
{
    void setup_simulator_input_file(string);
    
    public:
    simulation();
    ~simulation();
   
    void rebuild_simulator();
    bool execute_simulation(string);
};

#endif
