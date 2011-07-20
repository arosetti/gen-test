#include "population.h"

population::population(config *c)
{
    if (c)
        conf = c;
    else
    {
        cout << "null config pointer!" << endl;
        exit(0);
    }

    size = conf->avg_population_size;

    if (conf->gene_length > 0)
        gene_l = conf->gene_length;
    else
    {
        cout << "can't use zero gene length" << endl;
        exit(0);
    }

    if (conf->chromosome_length > 0)
        chromosome_l = conf->chromosome_length;
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
    for (it = pool.begin(); it!=pool.end(); ++it)
    {
        if (count == rnd)
        {
            i= *it;
            break;
        }
        count++;
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
    
    for (it = pool.begin(); it!=pool.end(); ++it)
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
    for(int i=0; i<(rand()%100+1); i++) // temporaneo per testare kill_individuals
    {
        pool.push_back(new_random_individual());    
    }
}


void population::mutate_individuals(uint32 strength) const
{
    list<individual*>::const_iterator it = pool.begin();
    float mutate_probability = conf->mutation_rate*100;
    float rnd;
    uint32 count = 0 ;
    
    for (it = pool.begin(); it!=pool.end(); ++it)
    {
        rnd = rand()%100 + 1; 
        
        //if (conf->debug)
        //    cout << "mutation_iteration: " << mutate_probability << ">" << rnd << "?" << endl;
        
        if (mutate_probability > rnd)
        {
            //if (conf->debug)
            //    cout << "mutation_before:"<< endl << (*it)->get_chromosome();

            (*it)->chromosome_mutate(conf->mutation_strength);

            //if (conf->debug)
            //    cout << "mutation_after:"<< endl << (*it)->get_chromosome();
        }
        count++;
    }  
}

void population::kill_individuals(uint32 rate)
{
    list<individual*>::iterator it = pool.begin();
    if (pool.size() > size)
    {
        if (conf->verbose && conf->print_kills)
            cout << "killing: " << (pool.size()-size) << endl;
        
        advance(it,size);
        pool.erase(it, pool.end());
    }
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

