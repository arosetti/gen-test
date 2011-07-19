#include <list>
#include <assert>

#include "individual.h"



#ifndef POPULATION_H
#define POPULATION_H

using namespace std;

class population
{   
    uint size;
    uint gene_len,chromosome_len;
    
    list<individual*> pool;
    
    void crossover(individual*, individual*);
    individual* get_random_individual();
    individual* new_random_individual();
    
    public:
    population(uint);
    ~population();
    
    void generate_random_population();

    void get_fitness();
    
    void mate();
    void kill();
    
    void sort();
}

#endif
