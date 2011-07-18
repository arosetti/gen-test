#include <list>

#ifndef POPULATION_H
#define POPULATION_H


class population
{   
    int pop_size;
    list<individual> pool;


    pubilic:
    population();
    ~population();

    get_random();

    void crossover();
    void mutate();
    void kill();
}

#endif
