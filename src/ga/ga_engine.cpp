#include "ga_engine.h"

ga_engine::ga_engine()
{
    iteration = 0;
    srand(time(NULL));
}

ga_engine::~ga_engine()
{
    delete pop;
}

void ga_engine::init()
{   
    if (conf->verbose)
        cout << "init population" << endl;
    pop = new population();
    
    if (conf->verbose)
        cout << "selecting " << conf->population_size << " random individuals..." << endl;
    pop->new_random_population();
}


void ga_engine::evolve()
{
    float best_fitness = 0;

    if (!conf)
    {
        cout << "GA parameters aren't configured." << endl;
        return;
    }

    if (conf->verbose)
        cout << "the population is going to evolve for " << conf->max_iterations << \
            " generations!" << endl << endl;

    while ( iteration++ < conf->max_iterations ) 
    {
        if (conf->verbose)
            cout << endl << "iteration: " << iteration << endl;
        if (conf->verbose && conf->print_population_size)
            cout << endl << "population: " << pop->size() << endl;
            
        best_fitness = pop->get_best_fitness();
        
        if (conf->verbose && conf->print_best_fitness)
            cout << "best_fitness: " << best_fitness << endl;

        if (conf->verbose && conf->print_best_dna)
            pop->print_best();
            
        pop->mate_individuals();
        pop->kill_individuals(pop->size()-conf->population_size);

        pop->mutate_individuals();

        if (conf->verbose)
            cout << "starting calc_fitness" << endl;

        pop->calc_fitness();
        pop->sort_by_fitness();
        
        if (conf->verbose && conf->debug)
            pop->print_all();
    }
}
