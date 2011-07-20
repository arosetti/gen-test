#ifndef POPULATION_H
#define POPULATION_H

#include <cstdio>
#include <iostream>
#include <list>

#include "individual.h"
#include "config.h"

using namespace std;

class population
{
    uint size;
    uint gene_l, chromosome_l;

    list<individual*> pool;
    list<individual*> limbo;

    config *conf;

    individual* get_random_individual() const;
    individual* new_random_individual();
    void crossover(individual*, individual*);

    public:
    population(config *c);
    ~population();

    void    new_random_population();

    float   get_avg_fitness() const;
    float   get_best_fitness() const;
    void    sort_by_fitness();

    void    mate_individuals(uint32);
    void    mutate_individuals(uint32) const;
    void    kill_individuals(uint32);

    uint32  count_individuals() const;
    void    print_best_individual() const;
};

#endif
