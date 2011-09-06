#ifndef POPULATION_H
#define POPULATION_H

#include <cstdio>
#include <iostream>
#include <sstream>
#include <list>

#include "individual.h"
#include "../simulation/general_tests.h"

using namespace std;

typedef std::map<uint32, uint32>       weight_map;
typedef std::map<uint32, individual*>  individual_map;
typedef std::pair<uint32, individual*> individual_pair;
typedef std::list<uint32>              individual_id_list;

extern config *conf;

//! Contenitore della popolazione
/**
 *
 */

class population
{
    individual_map*     pool;
    individual_map*     temp_pool;
    individual_id_list  mating_pool;

    general_tests test;

    individual* get_random_individual() const;
    individual* new_random_individual();

    void    create_mating_pool();
    void    crossover(individual*&, individual*&);
    void    clear_population();

    public:
    population();
    ~population();

    void    new_random_population();

    void    test_population();
    void    reset_faults(); // ?? da inserire in general tests ??
    
    void    eval_fitnesses();
    float   get_avg_fitness() const;
    float   get_best_fitness() const;
    void    sort_by_fitness();  // deprecated

    void    transfer_bests();
    void    print_best() const;
    void    mate_individuals();
    void    mutate_individual(individual*);
    void    mutate_individuals() const;  // deprecated

    uint32  size() const;
    void    log(uint32 generation) const;
    void    print() const;

    // for Thread
    public:
    individual* get_next_ind();
    void dec_threads();   
    void inc_threads();  
    
    private:
    void  getlock_ind_itr()
    {
        pthread_mutex_lock(&mutex_ind_itr);
    }
    void  releaselock_ind_itr()
    {
        pthread_mutex_unlock(&mutex_ind_itr);
    }
    void  getlock_n_thread()
    {
        pthread_mutex_lock(&mutex_n_thread);
    }
    void  releaselock_n_thread()
    {
        pthread_mutex_unlock(&mutex_n_thread);
    }

    individual_map::const_iterator ind_itr;  // deve essere settato a ind_itr = pool->begin() prima dell'avvio dei thread
    uint32 n_thread;                         // deve essere messo pari al numero dei thread prima dell'avvio di essi
    pthread_mutex_t    mutex_ind_itr;
    pthread_mutex_t    mutex_n_thread;
};

#endif
