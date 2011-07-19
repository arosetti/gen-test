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

individual* population::get_random_individual() const
{
    individual *i= NULL;
    int rnd = rand()%size + 1,count=0;
    
    
    list<individual*>::const_iterator it = pool.begin();
    while((it != pool.end()) || (++count == rnd))
    {
        i= *it;
        it++;
    }  
    
    return i;
}

individual* population::new_random_individual()
{
    individual *i = new individual(gene_len,chromosome_len);
    
    if(i)
        i->chromosome_random();

    return i;
}

void population::new_random_population()
{
    int created = 0;
    
    while( (created++) < size )
        pool.push_back(new_random_individual());
}


float population::get_avg_fitness() const
{

}

float population::get_best_fitness() const
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


void population::kill_individuals(uint32 rate)
{
    /*
    list<individual*>::it=pool.begin();
    int kill_after_this = 
    it+=size
    pool.erase(it, pool.end());
    */
}

uint32  population::count_individuals() const
{
    return pool.size();
}

void population::print_best_individual()
{
    list<individual*>::iterator it = pool.begin();
    cout << (*it)->get_chromosome();
}

