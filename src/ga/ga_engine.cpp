#include "ga_engine.h"

ga_engine::ga_engine()
{
    generation = 0;
    srand(time(NULL));
}

ga_engine::~ga_engine()
{
    delete pop;
}

void ga_engine::init()
{
    if (conf->verbose)
        cout << "* init population" << endl;
    pop = new population();

    if (conf->verbose)
        cout << "* selecting " << conf->population_size << " random individuals..." << endl;
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
        cout << "* the population is going to evolve for " << conf->max_generations << \
            " generations!" << endl << endl;

    while ( generation++ < conf->max_generations )
    {
        if (conf->verbose)
            cout << endl << "* generation: " << generation << endl;
        if (conf->verbose && conf->print_population_size)
            cout << "* population: " << pop->size() << endl;
            
        if (conf->verbose)
            cout << "* calc_population_fitness" << endl;
        pop->calc_population_fitness();
        
        best_fitness = pop->get_best_fitness();
        if (conf->verbose && conf->print_best_fitness)
            cout << "* best_fitness: " << best_fitness << endl;

        if (conf->verbose && conf->print_best_dna)
            pop->print_best();

        if (conf->verbose)
            cout << "* mating_individuals" << endl;
        pop->mate_individuals();

        if (conf->verbose)
            cout << "* mutate_individuals" << endl;
        pop->mutate_individuals();

        if (conf->debug)
        {
            cout << "* logging generation to file" << endl;
            pop->log_population(generation);
        }
    }
}
