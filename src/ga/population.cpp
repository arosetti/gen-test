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

    pool = new individual_map;
}

population::~population()
{
    empty_population();
}

individual* population::get_random_individual() const
{
    if (!pool->size())
        return NULL;
    
    individual_map::const_iterator itr = pool->begin();
    advance(itr, rand()%pool->size());
    
    return (*itr).second;
}

individual* population::new_random_individual()
{   
    uint32 len = rand()%(conf->chromosome_start_len_max - \
                         conf->chromosome_start_len_min + 1 )  \
                         + conf->chromosome_start_len_min;
                         
    individual* ind = new individual(len,conf->chromosome_num);
    ind->dna_random();

    if(conf->verbose && conf->debug)
        cout << "new individual, chromosome_len = " << len << endl;

    return ind;
}

void population::new_random_population()
{
    int created = 0;

    while (created++ < conf->population_size)
        pool->insert(pool->end(),
                    individual_pair(created, new_random_individual()));
}

void population::empty_population()
{
    individual_map::iterator itr;
    
    for (itr = pool->begin(); itr != pool->end(); itr++)
    {
        if ((*itr).second) 
            delete (*itr).second;
    }
    
    pool->clear();
}

void population::calc_fitness()
{
    individual_map::const_iterator itr = pool->begin();    
    for (; itr != pool->end(); ++itr)
    {
        (*itr).second->calc_fitness();
    }  
}

float population::get_avg_fitness() const
{
    float sum_fitness = 0;

    if (!pool->size())
        return 0;

    individual_map::const_iterator itr = pool->begin();    
    for (; itr != pool->end(); ++itr)
    {
        sum_fitness += (*itr).second->get_fitness();
    }

    return sum_fitness/pool->size();
}

float population::get_best_fitness() const
{
    float best_fitness = 0;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        if (best_fitness < (*itr).second->get_fitness())
            best_fitness = (*itr).second->get_fitness();
    }
    return best_fitness;
}

void population::sort_by_fitness()
{
    //pool->sort();
}

void population::mate_individuals()
{
    individual_map *temp_pool = new individual_map;
    individual_id_list::const_iterator itr = mating_pool.begin();
    uint32 rnd_a, rnd_b;
    individual *ind_a, *ind_b;

    while (mating_pool.size())
    {
        rnd_a = rand()%mating_pool.size();
        rnd_b = rand()%mating_pool.size();
        
        // devo scorrere la lista e trovare gli id corrispondenti nella posizine rnd
        //rnd_a = (*mating_pool.find(mating_pool.begin(), mating_pool.end(),rnd_a));
        //rnd_b = (*mating_pool.find(mating_pool.begin(), mating_pool.end(),rnd_b));
        
        ind_a = (*pool->find(rnd_a)).second;
        ind_b = (*pool->find(rnd_b)).second;
        
        // TODO non si può usare i soliti puntatori devo essere ricreati gli oggetti
        crossover(ind_a, ind_b); 

        temp_pool->insert(temp_pool->end(),
                 individual_pair(temp_pool->size(), ind_a));
        temp_pool->insert(temp_pool->end(),
                 individual_pair(temp_pool->size(), ind_b));
    }

    delete pool;
    pool = temp_pool;
}

void population::crossover(individual *& ind_a, individual *& ind_b)
{

}

void population::mutate_individuals() const
{
    float mutate_probability = conf->mutation_rate*100;
    float rnd;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        rnd = rand()%100 + 1; 

        if (mutate_probability > rnd)
        {
            if (conf->verbose && conf->print_mutations)
                cout << "mutation event!"<<endl;

            (*itr).second->dna_mutate();
        }
    }  
}

uint32  population::size() const
{
    return pool->size();
}

void population::create_mating_pool()
{
    weight_map m_weight_map;
    uint32 total_weight = 0;

    mating_pool.clear();

    if (!pool->size())
        return;
    
    for (individual_map::const_iterator itr = pool->begin(); itr != pool->end(); ++itr)
    {
        uint32 u_fitness = uint32((*itr).second->get_fitness() * 1000);
        if (!u_fitness)
            continue;
  
        total_weight += u_fitness;
        m_weight_map[(*itr).first] = u_fitness;
    }

    if (!m_weight_map.size())
        return;   
   
    uint32 selected_weight;
    uint32 weight;
    weight_map::const_iterator itr;

    for (uint32 i = 0; i < conf->population_size; i++)
    {
         selected_weight = rand()%total_weight;
         weight = 0;
         
         for (itr = m_weight_map.begin(); itr != m_weight_map.end(); ++itr)
         {
             weight += itr->second;
             if (selected_weight < weight)
             {
                mating_pool.push_front(itr->first);
                break;
             }
         }
    }
}

void population::print_best() const
{   
    individual_map::const_iterator itr;
    individual* ind = NULL;
    float best_fitness = 0;
   
    if (!pool->size())
        return;
    
    for (itr = pool->begin(); itr != pool->end(); ++itr)
    {
        if (best_fitness < (*itr).second->get_fitness())
        {
            best_fitness = (*itr).second->get_fitness();
            ind  = (*itr).second;
        }
    }
    
    if(ind)
        cout << ind->get_dna();
}

void population::print_all(string logfile) const
{
    uint32 count = 0;
    string str;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {   
        str+= "individual: #";
        str+= count; 
        str+= "\n";
        str+= "fitness:     ";
        str+= (*itr).second->get_fitness();
        str+= "\n";
        str+= "dna: ";
        str+= "\n";
        str+= (*itr).second->get_dna();
        str+= "\n\n";
        count++;
    }

    LOG(logfile, str, false); 
}
