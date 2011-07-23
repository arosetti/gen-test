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

void population::crossover(individual *& ind_a, individual *& ind_b)
{

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

void population::create_mating_pool()
{
    weight_map m_weight_map;
    uint32 total_weight = 0;

    mating_pool.clear();

    if (!pool->size())
        return;
        
    if(conf->debug && conf->verbose && conf->print_mating)
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

    for (uint32 i = 0; i < conf->population_size * conf->mating_fraction; i++)
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

void population::mate_individuals()
{
    individual_map *temp_pool = new individual_map;
    individual_id_list::iterator itr_del_a, itr_del_b;
    uint32 m_size = mating_pool.size();
    uint32 rnd_a, rnd_b;
    individual *ind_a, *ind_b;
    individual *ind_a_cloned, *ind_b_cloned;
    individual *ind_a_child, *ind_b_child;

    while (mating_pool.size())
    {   
        /* randomizzo due elementi della mating_pool */
        rnd_a = rand()%mating_pool.size(); 
        rnd_b = rand()%mating_pool.size();

        /* prendo il valore degli elementi nella mating_pool e dell'iteratore degli indici */        
        rnd_a = get_id_from_mating_pool(rnd_a, itr_del_a);
        rnd_b = get_id_from_mating_pool(rnd_b, itr_del_b);

        /* prendo i puntatori a individual usando gli indici della mating_pool */
        ind_a = (*pool->find(rnd_a)).second;
        ind_b = (*pool->find(rnd_b)).second;
        
        /* clono i genitori */
        ind_a_cloned = new individual(*ind_a);
        ind_b_cloned = new individual(*ind_b);
        
        if(conf->debug && conf->verbose && conf->print_mating)
        {
            cout << "mating_pool size: " << mating_pool.size() << "/"<< m_size << endl;
            cout << "temp_pool size: " << temp_pool->size() << endl;
            cout << "selected id: " << rnd_a << "-"<< rnd_b << endl;
            cout << "creating parents"<< endl;
        }
        
        /* inserisco i genitori nella nuova map */
        temp_pool->insert(temp_pool->end(),
                 individual_pair(temp_pool->size(), ind_a_cloned));
        temp_pool->insert(temp_pool->end(),
                 individual_pair(temp_pool->size(), ind_b_cloned));
        
        /* se i genitori non sono lo stesso individual */       
        if(itr_del_a != itr_del_b)
        {   
            /* rimuovo gli indici dalla lista */
            mating_pool.erase(itr_del_a);
            mating_pool.erase(itr_del_b);
 
            /* effettuo il crossover */
            crossover(ind_a, ind_b);
            
            /* creo i puntatori ai figli */
            ind_a_child = new individual(*ind_a);
            ind_b_child = new individual(*ind_b);
            
            if(conf->debug && conf->verbose && conf->print_mating)
                cout << "creating childs" << endl;

            /* aggiungo i figli alla nuova map */
            temp_pool->insert(temp_pool->end(),
                     individual_pair(temp_pool->size(), ind_a_child));
            temp_pool->insert(temp_pool->end(),
                     individual_pair(temp_pool->size(), ind_b_child));
        }
        else 
        {
            /* se i genitori sono lo stesso individuo elimino l'id dalla lista */
            mating_pool.erase(itr_del_a);
        }
    }

    /* se la mating pool non era vuota svuoto la vecchia map e la sostituisco con la nuova */
    if (m_size)
    {
        empty_population();
        pool = temp_pool;
    }
    
    if(conf->debug && conf->verbose && conf->print_mating)
        cout << "temp_pool size: " << temp_pool->size() << endl;
}

void population::mutate_individuals() const
{
    individual_map::const_iterator itr;
    float mutate_probability = conf->mutation_rate * 100;
    float rnd;
 
    for (itr = pool->begin(); itr != pool->end(); ++itr)
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
    individual_map::const_iterator itr;
    uint32 count = 0;
    stringstream out;

    for (itr = pool->begin(); itr != pool->end(); ++itr)
    {   
        out << "individual: #" << count << endl;
        out << "ptr : " << (*itr).second << endl;
        out << "fitness:     " << (*itr).second->get_fitness() << endl;
        out << "dna: " << endl << (*itr).second->get_dna() << endl << endl;
        count++;
    }

    LOG(logfile, out.str(), false); 
}

void population::cout_all(string logfile) const
{   
    individual_map::const_iterator itr;
    uint32 count = 0;

    for (itr = pool->begin(); itr != pool->end(); ++itr)
    {   

        cout << "individual: #" << count << endl;
        cout << "ptr : " << (*itr).second << endl;
        cout << "fitness:     " << (*itr).second->get_fitness() << endl;
        cout << "dna: " << endl << (*itr).second->get_dna() << endl << endl;
        count++;
    }
}
