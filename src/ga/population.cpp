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

void population::calc_population_fitness()
{
    individual_map::const_iterator itr = pool->begin();    
    for (; itr != pool->end(); ++itr)
    {
        if((*itr).second)
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

uint32 population::get_id_from_mating_pool(uint32 pos, individual_id_list::iterator &itr_to_del)
{
    individual_id_list::iterator itr;
    uint32 i = 0;
    
    for (itr = mating_pool.begin(); itr!= mating_pool.end(); ++itr)
    {
        if (i == pos)
        {
            itr_to_del = itr;
            return (*itr);
        }
        i++;
    }
    
    return 0;
}

void population::mate_individuals()
{
    individual_map *temp_pool = new individual_map;
    individual_id_list::iterator itr, itr_del_a, itr_del_b;
    uint32 m_size = mating_pool.size();
    uint32 rnd_a, rnd_b;
    individual *ind_a, *ind_b;
    individual *ind_a_copy, *ind_b_copy;

    cout << "mating_pool.size() = " << m_size << endl;

    while (mating_pool.size())
    {
        rnd_a = rand()%mating_pool.size();
        rnd_b = rand()%mating_pool.size();

        cout << "mating_pool size: " << mating_pool.size() << "/"<< m_size << endl;
        cout << "selected rnd: " << rnd_a << "-"<< rnd_b << endl;
        
        rnd_a = get_id_from_mating_pool(rnd_a, itr_del_a);
        rnd_b = get_id_from_mating_pool(rnd_b, itr_del_b);
        
        if(itr_del_a != itr_del_b)
        {        
            mating_pool.erase(itr_del_a);
            mating_pool.erase(itr_del_b);
        }
        else
            mating_pool.erase(itr_del_a);
        
        cout << "selected id: " << rnd_a << "-"<< rnd_b << endl;
        
        ind_a = (*pool->find(rnd_a)).second;
        ind_b = (*pool->find(rnd_b)).second;

        crossover(ind_a, ind_b);

        ind_a_copy = new individual(1,1);
        ind_b_copy = new individual(1,1);
        cout << "new " << ind_a << "-"<< ind_b << endl;
        *ind_a_copy = *ind_a;
        *ind_b_copy = *ind_b;

        temp_pool->insert(temp_pool->end(),
                 individual_pair(temp_pool->size(), ind_a_copy));
        temp_pool->insert(temp_pool->end(),
                 individual_pair(temp_pool->size(), ind_b_copy));
    }

    if (m_size)
    {
        empty_population();
        pool = temp_pool;
    }
    cout << "exit_mate " << endl;
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
            if ((*itr).second)
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
        
    cout << "create_mating_pool" << endl;
    
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
    stringstream out;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {   
        out << "individual: #" << count << endl;
        out << "fitness:     " << (*itr).second->get_fitness() << endl;
        out << "dna: " << endl << (*itr).second->get_dna() << endl << endl;
        count++;
    }

    LOG(logfile, out.str(), false); 
}
