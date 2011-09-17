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
extern vector<pthread_t> threads_id;

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

    individual *best_individual;
    individual *worst_individual;

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
    void    eval_fitnesses();

    float   get_avg_fitness() const;
    float   get_avg_fault_coverage() const;
    float   get_avg_chromosome_length() const;
    float   get_max_fault_coverage() const;        //min?
    float   get_max_chromosome_length() const;     //min?
    const individual* get_best_individual();
    const individual* get_worst_individual();
    const individual* get_fattest_individual();

    void    transfer_best();
    void    mate_individuals();
    void    set_mutation_rate(float rate);
    void    fattest_individuals_shrink();

    uint32  size() const;
    string  to_string() const;
    void    print(individual*) const;
    void    log(uint32) const;
    int     load_log(string);

    individual* get_next_individual();
    void dec_threads();
    void inc_threads();
    void inc_barlink();
    
    private:

    inline void  mutex_init()
    {
        pthread_mutex_init(&mutex_ind_itr, NULL);
        pthread_mutex_init(&mutex_n_thread, NULL);
        pthread_mutex_init(&mutex_barlink, NULL);
    }

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
    pthread_mutex_t    mutex_ind_itr;
    pthread_mutex_t    mutex_n_thread;
    pthread_mutex_t    mutex_barlink;
    prog_bar p_bar;
};

#endif
