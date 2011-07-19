#ifndef POPULATION_H
#define POPULATION_H

#include <list>

#include "individual.h"

using namespace std;

class population
{   
    uint size;
    uint gene_len, chromosome_len;
    
    list<individual*> pool;
    list<individual*> limbo;
 
    individual* get_random_individual();
    individual* new_random_individual();
    void crossover(individual*, individual*);
    
    public:
    population(uint size);
    ~population();
    
    void    new_random_population();

    void    get_avg_fitness();
    void    get_best_fitness();
    void    sort_by_fitness();
        
    void    mate_individuals(uint32);
    void    mutate_individuals(uint32);
    void    kill_individuals();
    
    uint32  count_individuals();
    void    print_best_individual();
};

#endif
