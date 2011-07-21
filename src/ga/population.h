#ifndef POPULATION_H
#define POPULATION_H

#include <cstdio>
#include <iostream>
#include <list>

#include "individual.h"

using namespace std;

extern config *conf;

class population
{
    list<individual*> pool;
    list<individual*> mating_pool;

    individual* get_random_individual() const;
    individual* new_random_individual();
    void crossover(individual*, individual*);

    public:
    population();
    ~population();

    void    new_random_population();

    void    calc_fitness();
    float   get_avg_fitness() const;
    float   get_best_fitness() const;
    void    sort_by_fitness();

    void    mate_individuals();
    void    mutate_individuals() const;
    void    kill_individuals(uint32);

    uint32  size() const;
    void    print_best() const;
};

#endif
