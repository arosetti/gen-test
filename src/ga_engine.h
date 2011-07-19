#ifndef GA_ENGINE_H
#define GA_ENGINE_H

#include <cstdio>
#include <iostream>

#include "population.h"
#include "config.h"

using namespace std;

class ga_engine
{
    uint32 iteration;
    population *p;
    config *c;
    
    public:
    ga_engine();
    ~ga_engine();

    void init(config*);
    void evolve();
};

#endif
