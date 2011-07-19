#include "population.h"

population::population(uint n)
{
    size=n;
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

}

individual* population::new_random_individual()
{
    individual *i = new individual(gene_len,chromosome_len);
    
    if(i)
        i->set_random_chromosome();

    return i;
}

void population::generate_random_population()
{
    int created;
    
    while(created<size)
        pool.pushback(new_random_individual());
}

