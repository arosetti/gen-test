#include "population.h"

population::population()
{
    if (conf->population_size == 0)
    {
        cout << "can't use zero population size" << endl;
        exit(0);
    }
    
    if (conf->chromosome_start_len_min == 0 || conf->chromosome_start_len_max == 0)
    {
        cout << "can't use zero chromosome length" << endl;
        exit(0);
    }
    
    if (conf->chromosome_start_len_min > conf->chromosome_start_len_max)
    {
        cout << "chromosome_start_len_min value must be lower than chromosome_start_len_max value" << endl;
        exit(0);
    }
}

population::~population()
{
    for (individual_map::iterator it = pool.begin(); it != pool.end(); it++)
    {
        if (*it) 
            delete *it;
    }
}

individual* population::get_random_individual() const
{
    individual *i = NULL;
    int rnd = rand()%pool.size() + 1, count = 0;
    
    
    individual_map::const_iterator it = pool.begin();
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

individual population::new_random_individual()
{   
    uint32 len = rand()%(conf->chromosome_start_len_max - \
                         conf->chromosome_start_len_min + 1 )  \
                         + conf->chromosome_start_len_min;
                         
    individual ind(len,conf->chromosome_num);
    ind.dna_random();

    if(conf->verbose && conf->debug)
        cout << "new individual, chromosome_len = " << len << endl;

    return ind;
}

void population::new_random_population()
{
    int created = 0;

    while (created++ < conf->population_size)
        pool.insert(pool.end(), \
                    individual_pair(new_random_individual(),created));

}

void population::calc_fitness()
{
    individual_map::const_iterator it = pool.begin();
    
    for (it = pool.begin(); it!=pool.end(); ++it)
    {
        (*it).second.calc_fitness();
    }  
}

float population::get_avg_fitness() const
{
    float sum_fitness = 0;
    individual_map::const_iterator it = pool.begin();
    
    for (it = pool.begin(); it!=pool.end(); ++it)
    {
        sum_fitness+=(*it).second.get_fitness();
    }  

    if (pool.size() == 0)
        return 0;
    else 
        return sum_fitness/pool.size();
}

float population::get_best_fitness() const
{
    individual_map::const_iterator it = pool.begin();
    return (float)(*it).second.get_fitness();
}

void population::sort_by_fitness()
{
    pool.sort();
}

void population::mate_individuals()
{
    for (int i=0; i<(rand()%100+1); i++) // temporaneo per testare kill_individuals
    {
        pool.insert(pool.end(), \
                    individual_pair(new_random_individual(),pool.size()));
    }
}

void population::mutate_individuals() const
{
    individual_map::const_iterator it = pool.begin();
    float mutate_probability = conf->mutation_rate*100;
    float rnd;
    
    for (it = pool.begin(); it!=pool.end(); ++it)
    {
        rnd = rand()%100 + 1; 
        
        if (mutate_probability > rnd)
        {
            if (conf->verbose && conf->print_mutations)
                cout << "mutation event!"<<endl;

            (*it).second.dna_mutate();
        }
    }  
}

void population::kill_individuals(uint32 howmany)
{
    individual_map::iterator it = pool.begin();
    if (pool.size() > conf->population_size)
    {
        if (conf->verbose && conf->print_kills)
            cout << "killing: " << (pool.size() - conf->population_size) << endl;
        
        advance(it, conf->population_size);
        pool.erase(it, pool.end());
    }
}

uint32  population::size() const
{
    return pool.size();
}

void population::create_mating_pool()
{
/*
    weight_map m_weight_map;
    uint32 total_weight = 0;
    uint32 selected_weight = 0;
    uint32 id = 0;
    uint32 weight;
    
    // inserisci i vari elementi nella mappa con la fitness o un numero dipendente da essa
    m_weight_map[id] = selected_weight;
     
    // se non ci sono elementi
    if (m_weight_map.size())
      return;

    // Somma tutti i vari pesi (può essere anche calcolato prima per ottimizzare quando viene aggiunto alla mappa)
    for (weight_map::const_iterator it = m_weight_map.begin(); it != m_weight_map.end(); ++it)
        total_weight += it->second;

    // Peso totale 0 si ritorna, non si sa mai
    if (!total_weight)
        return;
        
    selected_weight = rand(0, weight-1);
    weight = 0;
    for (weight_map::const_iterator it = m_weight_map.begin(); it != m_weight_map.end(); ++it)
    {
        weight += it->second;
        if (selected_weight < weight)
        {
            id = it->first;
            break;
        }
    }  
*/ 
}

void population::print_best() const
{
    individual_map::const_iterator it = pool.begin();
    cout << (*it).second.get_dna();
}

void population::print_all(string logfile) const
{
    individual_map::const_iterator it = pool.begin();
    uint32 count = 0;
    string str;

    for (it = pool.begin(); it!=pool.end(); ++it)
    {   
        str+= "individual: #";
        str+= count; 
        str+= "\n";
        str+= "fitness:     ";
        str+= (*it).second.get_fitness();
        str+= "\n";
        str+= "dna: ";
        str+= "\n";
        str+= (*it).second.get_dna();
        str+= "\n\n";
        count++;
    }
    
    LOG(logfile, str, false); 
}
