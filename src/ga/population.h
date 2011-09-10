#ifndef POPULATION_H
#define POPULATION_H

#include <cstdio>
#include <iostream>
#include <sstream>
#include <list>

#include "individual.h"
#include "../simulation/general_tests.h"
#include "../common/progress_bar/prog_bar.h"

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

    float mutation_rate;

    individual* get_random_individual() const;
    individual* new_random_individual();
    individual* get_fattest_individual();

    void    create_mating_pool();
    void    crossover(individual*&, individual*&);
    void    clear_population();

    public:
    population();
    ~population();

    void    new_random_population();

    void    test_population();

    void    eval_fitnesses();
    float   get_avg_fitness() const;
    float   get_avg_chromosome_length() const;
    float   get_best_fitness() const;
  //void    sort_by_fitness();  // deprecated

    void    transfer_bests();
    void    print_best() const;
    void    mate_individuals();
    void    set_mutation_rate(float rate);
  //void    mutate_individuals() const;  // deprecated
    void    fattest_individual_shrink();

    uint32  size() const;
    void    log(uint32) const;
    void    load_log(uint32);
    void    print() const;

    // for threads
    public:
    individual* get_next_ind();
    void dec_threads();
    void inc_threads();
    void inc_barlink();
    
    private:

    inline void  getlock_ind_itr()
    {
        pthread_mutex_lock(&mutex_ind_itr);
    }
    inline void  releaselock_ind_itr()
    {
        pthread_mutex_unlock(&mutex_ind_itr);
    }
    inline void  getlock_n_thread()
    {
        pthread_mutex_lock(&mutex_n_thread);
    }
    inline void  releaselock_n_thread()
    {
        pthread_mutex_unlock(&mutex_n_thread);
    }
    inline void  getlock_pbar()
    {
        pthread_mutex_lock(&mutex_barlink);
    }
    inline void  releaselock_pbar()
    {
        pthread_mutex_unlock(&mutex_barlink);
    }

    individual_map::const_iterator ind_itr;
    int n_thread;
    prog_bar p_bar;
    pthread_mutex_t    mutex_ind_itr;
    pthread_mutex_t    mutex_n_thread;
    pthread_mutex_t    mutex_barlink;
    
};

#endif
