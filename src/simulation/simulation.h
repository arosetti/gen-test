#ifndef SIMULATION_H
#define SIMULATION_H

#include "simulation_setup.h"

using namespace std;

extern config *conf;

class simulation
{
    uint32 n_total_faults;
    uint32 n_fautls;

    bool setup_input_file(string);
    string read_output();

    public:
    simulation();
    ~simulation();

    void get_results(uint32&, uint32&);
    bool execute(string);
    void rebuild();
    void patch();
};

#endif
