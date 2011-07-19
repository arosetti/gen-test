#include "ga_engine.h"

ga_engine::ga_engine()
{
    srand(time(NULL));
}

ga_engine::~ga_engine()
{

}

void ga_engine::init()
{
    p.generate_random_population();
}

void ga_engine::evolve()
{
	for (int i=0; i<max_iterations; i++) 
	{
		p.calc_fitness();
		p.sort();
		p.print_best();
		p.crossover();
	}

}
