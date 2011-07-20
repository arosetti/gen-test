#include "population.h"

population::population(uint32 size, uint32 c_l, uint32 g_l)
{
    population::size=size;
    
    if (g_l > 0)
        gene_l = g_l;
    else
    {
        cout << "can't use zero gene length" << endl;
        exit(0);
    }
    
    if (g_l > 0)
        chromosome_l = c_l;
    else
    {
        cout << "can't use zero chromosome length" << endl;
        exit(0);
    }
}

population::~population()
{
    for (list<individual*>::iterator it = pool.begin(); it != pool.end(); it++)
    {
        if (*it) 
            delete *it;
    }
}

individual* population::get_random_individual() const
{
    individual *i = NULL;
    int rnd = rand()%pool.size() + 1, count = 0;
    
    
    list<individual*>::const_iterator it = pool.begin();
    while (it != pool.end())
    {
        if (count == rnd)
        {
            i= *it;
            break;
        }
        count++;
        it++;
    }  
    
    return i;
}

individual* population::new_random_individual()
{
    individual *i = new individual(gene_l, chromosome_l);
    
    if(i)
        i->chromosome_random();

    return i;
}

void population::new_random_population()
{
    int created = 0;
    
    while (created++ < size)
        pool.push_back(new_random_individual());
}

float population::get_avg_fitness() const
{
    float sum_fitness = 0;
    list<individual*>::const_iterator it = pool.begin();
    
    while (it++ != pool.end())
    {
        sum_fitness+=(*it)->get_fitness();
    }  
    
    if (pool.size() == 0)
        return 0;
    else 
        return sum_fitness/pool.size();
}

float population::get_best_fitness() const
{
    list<individual*>::const_iterator it = pool.begin();
    return (float)(*it)->get_fitness();
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

void population::print_best_individual() const
{
    list<individual*>::const_iterator it = pool.begin();
    cout << (*it)->get_chromosome();
}

