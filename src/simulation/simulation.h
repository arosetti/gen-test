#ifndef SIMULATION_H
#define SIMULATION_H

#include <unistd.h>
#include <cstdlib>
#include <string>

#include "../common/utility.h"

using namespace std;

extern config *conf;

class simulation
{
    void write_simulation_input();
    
    public:
    simulation();
    ~simulation();
   
    void rebuild_simulator();
    void execute_simulation(string);
};

#endif
