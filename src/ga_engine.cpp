#include "ga_engine.h"

ga_engine::ga_engine()
{
    iteration = 0;
    srand(time(NULL));
}

ga_engine::~ga_engine()
{

}

void ga_engine::init(config *c)
{   
    ga_engine::c = c;
    
    if(c->verbose)
        cout << "init population" << endl;
    p = new population(c->avg_population_size, c->gene_length, c->chromosome_length);
    
    if(c->verbose)
        cout << "selecting " << c->avg_population_size << " random individuals..." << endl;
    p->new_random_population();
}


void ga_engine::evolve()
{
    float best_fitness = 0;
    
    if(!c)
    {
        cout << "GA parameters aren't configured." << endl;
        return;
    }
    
    if(c->verbose)
        cout << "the population is going to evolve for " << c->max_iterations << \
            " generations!" << endl << endl;
    
	while ( iteration++ < c->max_iterations ) 
	{
		best_fitness = p->get_best_fitness();
		
		if(c->verbose && c->print_best_fitness)
		    cout << "best_fitness: " << best_fitness << endl;

		p->sort_by_fitness();
		
		if(c->verbose && c->print_best_chromosome)
		    p->print_best_individual();
		    
		p->mate_individuals(c->mate_rate);
		p->kill_individuals(c->kill_rate);
		p->mutate_individuals(c->mutation_rate);
		
	}

}
