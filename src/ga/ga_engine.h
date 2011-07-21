#ifndef GA_ENGINE_H
#define GA_ENGINE_H

#include <cstdio>
#include <iostream>

#include "population.h"

using namespace std;

extern config *conf;

class ga_engine
{
    uint32 iteration;
    population *pop;

    public:
    ga_engine();
    ~ga_engine();

    void init();
    void evolve();
};

#endif
