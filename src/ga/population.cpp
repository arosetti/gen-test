#include "population.h"
#include <unistd.h>

population::population()
{
    if (conf->verbose && conf->debug)
        cout << "* using " << conf->chromosome_num << " chromosome(s)" << endl;

    pool = new individual_map;
    temp_pool = NULL;
    n_thread = 0;
    mutation_rate = 0.0f;
    best_fitness = 0.0f;
    worst_fitness = 0.0f;

    pthread_mutex_init(&mutex_ind_itr, NULL);
    pthread_mutex_init(&mutex_n_thread, NULL);
    pthread_mutex_init(&mutex_barlink, NULL);
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

individual* population::get_fattest_individual()
{
    individual_map::iterator itr = pool->begin();
    individual *fattest_ind = NULL;
    uint32      c_len = 0;

    for (; itr != pool->end(); ++itr)
    {
        if (c_len < (*itr).second->get_chromosome_length())
        {
            fattest_ind = (*itr).second;
            c_len = fattest_ind->get_chromosome_length();
        }
    }

    return fattest_ind;
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
    best_fitness = 0.0f;
    worst_fitness = 0.0f;

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
    n_thread = 0;

    if (conf->print_progress_bar)
        p_bar.init(pool->size());

    if (ind_itr != pool->end())
        for (int i = 0; i < conf->thread_slots; i++)
        {
            thread_params* t_params = new thread_params;
            t_params->pop = this;
            t_params->g_test = &test;
            t_params->sim_id = i;

            pthread_t tid;
            pthread_attr_t tattr;
            int ret;

            // per riusare i thread R.I.P.
            ret = pthread_attr_init(&tattr);
            ret = pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);

            if (ret = pthread_create(&tid, &tattr, SimulationThread, (void*)t_params))
            {
                perror("pthread_create ");
                delete t_params;
                continue;
            }
            else
                inc_threads();
        }

    while (n_thread)
    {
        usleep(30); // in millisecondi
    }

    cout << endl;

    /*
    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        if ((*itr).second)
            (*itr).second->ExecuteTest(&test);
    }
    */
}

float population::get_avg_chromosome_length() const
{
    float sum_length = 0;

    if (!pool->size())
        return 0;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        sum_length += (*itr).second->get_chromosome_length();
    }

    return sum_length/pool->size();
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

float population::get_best_fitness()
{
    if (best_fitness)
        return best_fitness;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        if (best_fitness < (*itr).second->get_fitness())
            best_fitness = (*itr).second->get_fitness();
    }
    return best_fitness;
}

float population::get_worst_fitness()
{
    if (worst_fitness)
        return worst_fitness;

    worst_fitness = get_best_fitness();

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        if (worst_fitness > (*itr).second->get_fitness())
            worst_fitness = (*itr).second->get_fitness();
    }
    return worst_fitness;
}


float population::get_best_fault_coverage()
{
    float best_fault_coverage = 0.0f;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        if (best_fault_coverage < (*itr).second->get_fault_coverage())
            best_fault_coverage = (*itr).second->get_fault_coverage();
    }
    return best_fault_coverage;
}

float population::get_best_chromosome_length()
{
    float best_chromosome_length = 0.0f;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        if (best_chromosome_length < (*itr).second->get_chromosome_length())
            best_chromosome_length = (*itr).second->get_chromosome_length();
    }
    return best_chromosome_length;
}

/*
void population::sort_by_fitness()  // deprecated
{
    //pool->sort();
}
*/

void population::crossover(individual *& ind_a, individual *& ind_b)
{
    uint32 cut_a,cut_b;
    string dna_a_1, dna_a_2;
    string dna_b_1, dna_b_2;

    if (conf->cut_type == "double_random")
    {
        cut_a = randmm(1, ind_a->get_chromosome_length());
        cut_b = randmm(1, ind_b->get_chromosome_length());
    }
    else if (conf->cut_type == "single_random")
    {   
        uint32 cmin = min((int)ind_a->get_chromosome_length(), (int)ind_b->get_chromosome_length());
        cut_a = cut_b = randmm(1, cmin);
    }
    else if (conf->cut_type == "half")
    {
        cut_a = ind_a->get_chromosome_length()/2;
        cut_b = ind_b->get_chromosome_length()/2;
    }
    
    if ( conf->debug && conf->log_mating)
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

    if (conf->debug && conf->verbose && conf->log_mating)
        cout << "create_mating_pool" << endl;

    for (individual_map::const_iterator itr = pool->begin(); itr != pool->end(); ++itr)
    {
        float fitness = (*itr).second->get_fitness();
        if (!fitness)
            continue;

        if (conf->normalized_fitness)  // Normalized
            if (get_best_fitness() - get_worst_fitness() != 0.0f)            
                fitness = (fitness - get_worst_fitness()) / (get_best_fitness() - get_worst_fitness());

        uint32 u_fitness = uint32(fitness * 1000);        

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
         selected_weight = randmm(0,total_weight);
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

    transfer_num = transfer_num ? transfer_num : 1;

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
        if (conf->log)
            LOG->log("logs/mating.log",true, "mating_pool is empty\n");
        return;
    }
    individual_id_list::iterator itr = mating_pool.begin();

    uint32 index = 0;

    // Inizio accoppiamento
    while (mating_pool.size() > (index + 1))
    {
        // Clono gli individual che faranno parte della nuova map
        ind_a_cloned = new individual(*(*pool->find(*itr++)).second);
        ind_b_cloned = new individual(*(*pool->find(*itr++)).second);

        // Effettuo il crossover con una certa probabilità
        if (randmm(0,100) <= (conf->mating_rate*100))
        {
            if (conf->debug && conf->verbose && conf->log_mating)
                cout << "crossover event!" << endl;

            crossover(ind_a_cloned, ind_b_cloned);

            // Dopo il crossover ci potrebbero essere delle mutazioni
            //ind_a_cloned->dna_mutate(mutation_rate);
            //ind_a_cloned->dna_mutate(mutation_rate);
        }

        ind_a_cloned->dna_mutate(mutation_rate);
        ind_a_cloned->dna_mutate(mutation_rate);

        // Aggiungo i figli alla nuova map
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

void population::set_mutation_rate(float rate)
{
    if (rate > 1.0f)
        mutation_rate = 1;
    else if (rate < 0.0f)
        mutation_rate = 0;
    else 
        mutation_rate = rate;
}

void population::fattest_individuals_shrink()
{
    individual_map::const_iterator itr = pool->begin();

    for (; itr != pool->end(); ++itr)
    {
         while((*itr).second->get_chromosome_length() >= conf->chromosome_max_len)
            (*itr).second->dna_shrink();
    }
}

/*
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

    if (conf->verbose && conf->log_mutations)
        cout << count <<" mutation events!"<<endl;
}
*/

uint32  population::size() const
{
    return pool->size();
}

void population::print_best() const
{
    individual_map::const_iterator itr;
    individual* ind = NULL;
    float best_fitness = 0;
    uint32 shortest = 0;

    if (!pool->size())
        return;

    for (itr = pool->begin(); itr != pool->end(); ++itr)
    {
        if (best_fitness <= (*itr).second->get_fitness())
        {
            // tra due individui con fitness uguali scelgo il più corto 
            if (ind && (*itr).second->dna_length() > ind->dna_length())
                continue;
            best_fitness = (*itr).second->get_fitness();
            ind  = (*itr).second;
        }
    }

    if (ind)
        cout << ind->info(false);
}

void population::log(uint32 generation) const
{
    string generations_logfile;
    individual_map::const_iterator itr;
    uint32 count = 0;
    stringstream out;

    generations_logfile = conf->log_path;
    addslash(generations_logfile);
    generations_logfile += "generation";
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

    LOG->log_static(generations_logfile, false, out.str().c_str());
}

void population::load_log(string filename)
{
    string temp_dna;
    individual* ind;
    
    ifstream log_file;
    char *buffer = NULL, *p_buffer = NULL;
    int length = 0, created = 0;

    clear_population();
    pool = new individual_map;
    temp_pool = NULL;

    log_file.open(filename.c_str(), ios::binary);

    if (!log_file.is_open())
    {
        printf("file: %s\n", filename.c_str());
        perror("log file");
        return;
    }

    log_file.seekg (0, ios::end);
    length = log_file.tellg();
    log_file.seekg (0, ios::beg);

    buffer = new char[length + 1];

    log_file.read (buffer,length);
    log_file.close();
    buffer[length] = 0;

    p_buffer = buffer;
    while (p_buffer)
    {
        temp_dna = "";
        p_buffer = strchr(p_buffer, '\n');

        if (p_buffer == 0)
            break;

        p_buffer++;
        if(strncmp(p_buffer, "dna", 3) == 0)
        {
            p_buffer = strchr(p_buffer, '\n');
            p_buffer++;
        }
        else
           continue;

        while(p_buffer[0] == '0' || p_buffer[0] == '1' || p_buffer[0] == '\n' )
        {
            temp_dna += p_buffer[0];
            p_buffer++;
        }
        
        if (temp_dna != "")
        {
            while (temp_dna.at(temp_dna.length()-1)  == '\n')
                temp_dna.resize(temp_dna.length()-1);

            cout << GetStrRowSize(temp_dna) << "-" << GetStrColSize(temp_dna) << endl;
            ind = new individual(GetStrRowSize(temp_dna), GetStrColSize(temp_dna));
            ind->set_dna(temp_dna);
            pool->insert(pool->end(),
                        individual_pair(created, ind));
        }

        created++;
    }
    delete[] buffer;
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

void population::inc_barlink()
{
    if (!conf->print_progress_bar)
        return;

    getlock_pbar();
    p_bar.step();
    releaselock_pbar();
}
