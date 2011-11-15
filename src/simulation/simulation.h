#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef SIMULATION_H
#define SIMULATION_H

#include "simulation_utility.h"

using namespace std;

extern config *conf;

class simulation
{
    uint32 n_total_faults;
    uint32 n_fautls;
    double propagation_factor;

    bool setup_input_file(string,uint32);

    public:
    simulation();
    ~simulation();

    void get_results(uint32, uint32&, uint32&);
    void get_results(uint32, uint32&, uint32&, double&);
    bool execute(string,uint32);
    void rebuild();
    void patch();
};

#endif
