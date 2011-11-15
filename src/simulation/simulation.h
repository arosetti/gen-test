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
    bool setup_input_file(string,uint32);

    public:
    simulation();
    ~simulation();

    void get_results(uint32, uint32&, uint32&);
    void get_results(uint32, uint32&, uint32&, float&);
    bool execute(string,uint32);
    void rebuild();
    void patch();
};

#endif
