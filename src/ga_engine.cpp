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

void ga_engine::init(config *c)
{   
    conf = c;

    if (conf->verbose)
        cout << "init population" << endl;
    pop = new population(conf);
    
    if (conf->verbose)
        cout << "selecting " << conf->avg_population_size << " random individuals..." << endl;
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
	
		best_fitness = pop->get_best_fitness();
		
		if (conf->verbose && conf->print_best_fitness)
		    cout << "best_fitness: " << best_fitness << endl;

		pop->sort_by_fitness();
		
		if (conf->verbose && conf->print_best_chromosome)
		    pop->print_best_individual();
		    
		pop->mate_individuals(conf->mate_rate);
		pop->kill_individuals(conf->kill_rate);
		pop->mutate_individuals(conf->mutation_rate);
		
	}
}
