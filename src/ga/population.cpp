#include "population.h"

population::population()
{   /*TODO controlli da fare nel config con una specifica funzione che decide se sono coerenti*/
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
    
    conf->chromosome_num = read_n_inputs();
    if (!conf->chromosome_num)
    {
        cout << "chromosome number is 0, errror reading simulator output.net" << endl;
        exit(0);
    }
    else if (conf->verbose)
        cout << "* using " << conf->chromosome_num << " chromosome(s)" << endl;

    pool = new individual_map;
    temp_pool = NULL;

    pthread_mutex_init(&mutex_ind_itr, NULL);
    pthread_mutex_init(&mutex_n_thread, NULL);
}

population::~population()
{
    clear_population();
}

individual* population::get_random_individual() const
{
    if (!pool->size())
        return NULL;

    individual_map::const_iterator itr = pool->begin();
    advance(itr, randmm(0,pool->size()));

    return (*itr).second;
}

individual* population::new_random_individual()
{
    uint32 len = randmm(conf->chromosome_start_len_min, 
                        conf->chromosome_start_len_max);

    individual* ind = new individual(len, conf->chromosome_num);
    ind->dna_random();

    return ind;
}

void population::new_random_population()
{
    for (int created = 0; created < conf->population_size; created++)
    {
        pool->insert(pool->end(),
                    individual_pair(created, new_random_individual()));
    }
    
    if (conf->verbose && conf->debug)
    {
        individual_map::iterator itr = pool->begin();
        cout << "* individual lengths: " << endl;
        for (; itr != pool->end(); ++itr)
        {
            if ((*itr).second)
                  cout << (*itr).second->get_chromosome_length() << " ";
        }
        cout << endl;
    }
}

void population::clear_population()
{
    individual_map::iterator itr;

    for (itr = pool->begin(); itr != pool->end(); itr++)
    {
        if ((*itr).second)
            delete (*itr).second;
    }

    pool->clear();
    if (pool)
        delete pool;
    pool = NULL;
}

void population::eval_fitnesses()
{
    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        if ((*itr).second)
            (*itr).second->calc_fitness();
    }
}

void population::test_population()
{
    ind_itr = pool->begin();

/*
    if (ind_itr != poll->end())
        for (int i = 0; i < conf->thread_slots; i++)
        {
            inc_threads();


        }*/
    
    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        if ((*itr).second)
            (*itr).second->ExecuteTest(&test);
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
    uint32 cut_a,cut_b;
    string dna_a_1, dna_a_2;
    string dna_b_1, dna_b_2;

    cut_a = rand()%(ind_a->get_chromosome_length()-1) + 1;
    cut_b = rand()%(ind_b->get_chromosome_length()-1) + 1;
    
    if ( conf->debug && conf->print_mating)
    {
        cout << "split " << cut_a << "/" << ind_a->get_chromosome_length();
        cout << "," << cut_b << "/" << ind_b->get_chromosome_length() << endl;
        cout << ind_a->info() << endl;
        cout << ind_b->info() << endl;
    }
    
    ind_a->dna_split(cut_a, dna_a_1, dna_a_2);
    ind_b->dna_split(cut_b, dna_b_1, dna_b_2);

    ind_a->dna_merge(dna_a_1, dna_b_2);
    ind_b->dna_merge(dna_b_1, dna_a_2);
}

void population::create_mating_pool()
{
    weight_map m_weight_map;
    uint32 total_weight = 0;

    mating_pool.clear();

    if (!pool->size())
        return;

    if (conf->debug && conf->verbose && conf->print_mating)
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

    for (uint32 i = 0; i < uint32(conf->population_size * conf->mating_fraction); i++)
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

void population::transfer_bests()
{
    uint32 transfer_num = uint32(conf->population_size) - uint32(conf->population_size * conf->mating_fraction);

    if (conf->mating_fraction == 1)
        return;

    transfer_num = (transfer_num == 0)?1:transfer_num;

    if (conf->debug)
        cout << "transferring " << transfer_num << " best individual(s) from old population" << endl;

    typedef std::pair<individual*, float> best_pair;
    std::list<best_pair> best_map;
    
    for (individual_map::const_iterator itr = pool->begin(); itr != pool->end(); ++itr)
    {
        for (std::list<best_pair>::iterator itr2 = best_map.begin();; ++itr2)
        {  
            if (itr2 == best_map.end() || (*itr2).second < (*itr).second->get_fitness())
            {        
                if (itr2 == best_map.end() && best_map.size() >= transfer_num)
                    break;               
                
                best_map.insert(itr2, best_pair((*itr).second, (*itr).second->get_fitness()));
                
                if (best_map.size() > transfer_num)
                {   
                    itr2 = best_map.end();
                    best_map.erase(--itr2);
                }

                /*for (std::list<best_pair>::iterator itr3 = best_map.begin(); itr3 != best_map.end(); ++itr3)
                {
                    cout<< (*itr3).second << " ";
                }            
                cout << endl << "best_map.size(): " << best_map.size() << endl;*/

                break;
            }
        }
    }

    if (!temp_pool)
        temp_pool = new individual_map;
    
    for (std::list<best_pair>::iterator itr2 = best_map.begin(); itr2 != best_map.end(); ++itr2)
    {
        individual *ind_cloned = new individual(*((*itr2).first));
        temp_pool->insert(temp_pool->end(), individual_pair(temp_pool->size(), ind_cloned));
    }
}

void population::mate_individuals()
{
    if (!temp_pool)
        temp_pool = new individual_map;
    individual *ind_a_cloned, *ind_b_cloned;

    create_mating_pool();

    if (mating_pool.empty())
    {
        if (conf->debug)
            cout << "mating_pool is empty" << endl;
        return;
    }
    individual_id_list::iterator itr = mating_pool.begin();


    uint32 index = 0;

    /* Inizio accoppiamento */
    while (mating_pool.size() > (index + 1))
    {
        /* Clono gli individual che faranno parte della nuova map */
        ind_a_cloned = new individual(*(*pool->find(*itr++)).second);
        //cout << "faults " << ((*pool->find(*itr)).second)->GetFaultsSize() << endl;
        ind_b_cloned = new individual(*(*pool->find(*itr++)).second);
        /* Effettuo il crossover con una certa probabilità */
        if ((rand()%100 + 1) <= (conf->mating_rate*100))
        {
            if (conf->debug && conf->verbose && conf->print_mating)
                cout << "crossover event!" << endl;

            crossover(ind_a_cloned, ind_b_cloned);

            /* Dopo il crossover ci potrebbero essere delle mutazioni */
            mutate_individual(ind_a_cloned);
            mutate_individual(ind_a_cloned);
        }

        /* Aggiungo i figli alla nuova map */
        temp_pool->insert(temp_pool->end(),
                 individual_pair(temp_pool->size(), ind_a_cloned));
        temp_pool->insert(temp_pool->end(),
                 individual_pair(temp_pool->size(), ind_b_cloned));

        index +=2; 
    }

    /* Se ne rimane uno che non ha trovato moglie lo copio nella nuova map */
    if (mating_pool.size() == (index + 1))
    {
        ind_a_cloned = new individual(*(*pool->find(*itr)).second);
        temp_pool->insert(temp_pool->end(),
            individual_pair(temp_pool->size(), ind_a_cloned));
    }

    /* Elimino la vecchia generazione e la sostituisco con la nuova */
    clear_population();
    pool = temp_pool;
    temp_pool = NULL;
    mating_pool.clear();
}

void population::mutate_individual(individual *ind)
{
    float count = conf->mutation_strength * ind->dna_length();

    if (!ind)
        return;

    if (conf->verbose && conf->print_mutations)
        cout << count <<" mutation event!"<<endl;

    while(count--)
        if ((conf->mutation_rate * 100) > (rand()%100 + 1))
            ind->dna_mutate();
}

void population::mutate_individuals() const
{
    individual_map::const_iterator itr;
    float mutate_probability = conf->mutation_rate * 100;
    float rnd, count = 0;

    for (itr = pool->begin(); itr != pool->end(); ++itr)
    {
        rnd = rand()%100 + 1;

        if (mutate_probability > rnd)
        {
            count++;
            if ((*itr).second)
                (*itr).second->dna_mutate();
        }
    }

    if (conf->verbose && conf->print_mutations)
        cout << count <<" mutation events!"<<endl;
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
        if (best_fitness < (*itr).second->get_fitness()) // TODO scegliere il più corto
        {
            best_fitness = (*itr).second->get_fitness();
            ind  = (*itr).second;
        }
    }

    if (ind)
        cout << ind->info();
}

void population::log(uint32 generation) const
{
    string generations_logfile;
    individual_map::const_iterator itr;
    uint32 count = 0;
    stringstream out;
    
    generations_logfile = "logs/generation";
    out << generation;
    generations_logfile.append(out.str());
    generations_logfile.append(".log");
    out.str("");

    for (itr = pool->begin(); itr != pool->end(); ++itr)
    {
        out << "individual: #" << count << endl;
        out << (*itr).second->info() << endl;
        count++;
    }

    LOG(generations_logfile, out.str(), false);
}

void population::print() const
{
    individual_map::const_iterator itr;
    uint32 count = 0;

    for (itr = pool->begin(); itr != pool->end(); ++itr)
    {
        cout << "individual: #" << count << endl;
        cout << (*itr).second->info() << endl;
        count++;
    }
}

void population::reset_faults()
{
    //test.EmptyFaults();
    //for (individual_map::iterator itr = pool->begin(); itr != pool->end(); ++itr)
    //    (*itr).second->EmptyFaults();
    //test.LoadFile(pool);
}

individual* population::get_next_ind()
{
    getlock_ind_itr();
    individual* ind = NULL;
    if (ind_itr != pool->end())
    {
        ind = (*ind_itr).second;
        ind_itr++;
    }
    releaselock_ind_itr();
    return ind;
}

void population::dec_threads()
{
    getlock_n_thread();
    n_thread--;
    releaselock_n_thread();
}

void population::inc_threads()
{
    getlock_n_thread();
    n_thread++;
    releaselock_n_thread();
}
