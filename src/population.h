#include <list>

#include "individual.h"

#ifndef POPULATION_H
#define POPULATION_H


class population
{   
    uint size;
    list<individual> pool;


    pubilic:
    population(uint);
    ~population();

    get_random();

    void crossover(individual*, individual*);
    void kill();
}

#endif
