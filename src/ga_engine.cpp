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
    
    cout << "Selecting " << size << " random individuals..." << endl;
    p.new_random_population();
}


void ga_engine::evolve()
{
    ufloat best_fitness = 0;
    
    if(!c)
    {
        cout << "GA parameters aren't configured." << endl;
        return;
    }
    
	while ( iteration++ < max_iterations ) 
	{
		best_fitness = p.get_best_fitness();
		cout << "fitness: " << best_fitness << endl;
		p.sort_by_fitness();
		p.print_best_individual();
		p.mate_individuals(c->mate_rate);
		p.kill_individuals();
		p.mutate_individuals();
		
	}

}
