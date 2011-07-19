#include "population.h"

population::population(uint size)
{
    population::size=size;
}

population::~population()
{
    for(list<individual*>::iterator it = pool.begin(); it != pool.end(); it++)
    {
        if(*it) 
            delete *it;
    }
}

individual* population::get_random_individual()
{
    individual *i= NULL;
    int rnd = rand()%size + 1;
    
    list<individual*>::iterator it = pool.begin();
    while((it != pool.end()) || ((it - pool.begin()) == rnd))
    {
        i= &*it;
        it++;
    }  
    
    return i;
}

individual* population::new_random_individual()
{
    individual *i = new individual(gene_len,chromosome_len);
    
    if(i)
        i->set_random_chromosome();

    return i;
}

void population::new_random_population()
{
    int created = 0;
    
    while( (created++) < size )
        pool.pushback(new_random_individual());
}


void population::get_avg_fitness()
{

}

void population::get_best_fitness()
{

}


void population::sort_by_fitness()
{
    pool.sort();
}


    
void population::mate_individuals(uint32 how_many)
{

}


void population::mutate_individuals(uint32 strength)
{

}


void population::kill_individuals(uint32 how_many)
{

}

