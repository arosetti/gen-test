#include "population.h"
#include <unistd.h>
#include <libgen.h>

population::population()
{
    INFO("verbose", "* using %d chromosome(s)\n", conf->get_int_config(CONFIG_CHROMOSOME_NUM));

    pool = new individual_map;
    temp_pool = NULL;
    n_thread = 0;

    switch(conf->get_int_config(CONFIG_MUTATION_TYPE))
    {
        case MUTATION_STATIC:
        case MUTATION_STALL_CHECK:
            mutation_rate = conf->get_float_config(CONFIG_MUTATION_RATE);
        break;
        case MUTATION_DYNAMIC:
            mutation_rate = conf->get_float_config(CONFIG_MAX_MUTATION_RATE);
        break;
    }

    best_individual = NULL;
    worst_individual = NULL;

    mutex_init();
}

population::~population()
{
    clear_population();
    mutex_destroy();
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
    uint32 len = randmm(conf->get_int_config(CONFIG_CHROMOSOME_START_LEN_MIN),
                        conf->get_int_config(CONFIG_CHROMOSOME_START_LEN_MAX));

    individual* ind = new individual(len, conf->get_int_config(CONFIG_CHROMOSOME_NUM));
    ind->dna_random();

    return ind;
}

void population::new_random_population()
{
    individual *ind = NULL;

    INFO("verbose", "* individual lengths:\n");
    for (int created = 0; created < conf->get_int_config(CONFIG_POPULATION_SIZE); created++)
    {
        ind = new_random_individual();

        if (!ind)
            return;

        pool->insert(pool->end(), individual_pair(created, ind));
        INFO("verbose", "%d ", ind->get_chromosome_length());
    }

    INFO("verbose","\n");
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
    best_individual = NULL;
    worst_individual = NULL;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        if ((*itr).second)
            (*itr).second->calc_fitness(&test);
    }
}

void population::test_population()
{
    ind_itr = pool->begin();
    uint32 untested = size() - tested_size();
    n_thread = 0;
    pthread_t      tid[conf->get_int_config(CONFIG_THREAD_SLOTS)];
    pthread_attr_t tattr;
    int ret;
    void *status;

    // Reset General Tests
    test.EmptyFaults();

    if (conf->get_bool_config(CONFIG_PRINT_PROGRESS_BAR))
        p_bar.init(untested);

    ret = pthread_attr_init(&tattr);
    ret = pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);

    if (ind_itr != pool->end())
        for (int i = 0; i < conf->get_int_config(CONFIG_THREAD_SLOTS); i++)
        {
            thread_params* t_params = new thread_params;
            t_params->pop = this;
            t_params->g_test = &test;
            t_params->sim_id = i;

            if (ret = pthread_create(&tid[i], &tattr, SimulationThread, (void*)t_params))
            {
                perror("pthread_create");
                delete t_params;
                continue;
            }

            threads_id.push_back(tid[i]);
            inc_threads();
        }

    for (int i = 0; i < conf->get_int_config(CONFIG_THREAD_SLOTS); i++)
        pthread_join(tid[i], &status);

    pthread_attr_destroy(&tattr);

    INFO("verbose", "\n* %d/%d individuals tested\n", untested, size());
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

    return sum_length / (float) pool->size();
}

float population::get_avg_fault_coverage() const
{
    float sum_fault_coverage = 0;

    if (!pool->size())
        return 0;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        sum_fault_coverage += (*itr).second->GetFaultCoverage();
    }

    return sum_fault_coverage/pool->size();
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

float population::get_max_fault_coverage() const
{
    float best_fault_coverage = 0.0f;

    individual_map::const_iterator itr = pool->begin();
    for (; itr != pool->end(); ++itr)
    {
        if (best_fault_coverage < (*itr).second->GetFaultCoverage())
            best_fault_coverage = (*itr).second->GetFaultCoverage();
    }
    return best_fault_coverage;
}

float population::get_max_chromosome_length() const
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

const individual* population::get_best_individual()
{
    if (!best_individual)
        if (pool->size())
            for (individual_map::const_iterator itr = pool->begin(); itr != pool->end(); ++itr)
            {
                if (!best_individual || *best_individual < *((*itr).second))
                {
                    best_individual = (*itr).second;
                }
            }

    return best_individual;
}

const individual* population::get_worst_individual()
{ 
    if (!worst_individual)
    {
        if (individual* ind = (individual*) get_best_individual())
            worst_individual = ind;
        else
            return NULL;

        if (pool->size())
            for (individual_map::const_iterator itr = pool->begin(); itr != pool->end(); ++itr)
            {
                if (!worst_individual || *worst_individual > *((*itr).second))
                {
                    worst_individual = (*itr).second;
                }
            }
    }

    return worst_individual;
}

const individual* population::get_fattest_individual()
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

void population::crossover(individual *& ind_a, individual *& ind_b)
{
    uint32 cut_a_1, cut_a_2, cut_b_1, cut_b_2;
    string dna_a_1, dna_a_2, dna_a_3;
    string dna_b_1, dna_b_2, dna_b_3;

    if (conf->get_bool_config(CONFIG_LOG_MATING))
    {
        LOG_STATIC("mating_events", "crossover", "### crossover\n");
        LOG("mating_events", "crossover", "%s\n", ind_a->info().c_str());
        LOG("mating_events", "crossover", "%s\n", ind_b->info().c_str());
    }

    switch (conf->get_int_config(CONFIG_CUT_TYPE))
    {
        case CUT_DOUBLE_RANDOM:
        {
            cut_a_1 = randmm(0, ind_a->get_chromosome_length()-1);
            cut_b_1 = randmm(0, ind_b->get_chromosome_length()-1);  
        }
        break;
        case CUT_SINGLE_RANDOM:
        {
            uint32 cmin = min((int)ind_a->get_chromosome_length()-1, 
                              (int)ind_b->get_chromosome_length()-1);
            cut_a_1 = cut_b_1 = randmm(0, cmin);
        }
        break;
        case CUT_HALF:
        {
            cut_a_1 = (ind_a->get_chromosome_length()-1)/2;
            cut_b_1 = (ind_b->get_chromosome_length()-1)/2;
        }
        break;
        case CUT_TWO_POINTS_DOUBLE_RANDOM:
        {
            cut_a_1 = randmm(0, ind_a->get_chromosome_length()-1);
            cut_a_2 = randmm(0, ind_a->get_chromosome_length()-1);

            cut_b_1 = randmm(0, ind_b->get_chromosome_length()-1);
            cut_b_2 = randmm(0, ind_b->get_chromosome_length()-1); 
        }
        break;
        case CUT_TWO_POINTS_SINGLE_RANDOM:
        {
            uint32 cmin = min((int)ind_a->get_chromosome_length()-1,
                              (int)ind_b->get_chromosome_length()-1);
            cut_a_1 = cut_b_1 = randmm(0, cmin);
            cut_a_2 = cut_b_2 = randmm(0, cmin);
        }
        break;
        case CUT_END_DOUBLE_RANDOM:
        {
            cut_a_1 = u_exponential_random(0, ind_a->get_chromosome_length()-1,
                                           conf->get_float_config(CONFIG_CUT_END_EXPONENT));
            cut_b_1 = u_exponential_random(0, ind_b->get_chromosome_length()-1,
                                           conf->get_float_config(CONFIG_CUT_END_EXPONENT));
        }
        break;
        case CUT_END_SINGLE_RANDOM:
        {
            uint32 cmin = min((int)ind_a->get_chromosome_length()-1,
                              (int)ind_b->get_chromosome_length()-1);
            cut_a_1 = cut_b_1 = u_exponential_random(0, cmin,
                                conf->get_float_config(CONFIG_CUT_END_EXPONENT));
        }
        break;
    }

    switch (conf->get_int_config(CONFIG_CUT_TYPE))
    {
        case CUT_DOUBLE_RANDOM:
        case CUT_SINGLE_RANDOM:
        case CUT_HALF:
        case CUT_END_SINGLE_RANDOM:
        case CUT_END_DOUBLE_RANDOM:
        {
            if (conf->get_bool_config(CONFIG_LOG_MATING))
            {
                LOG("mating_events", "crossover", "ind_a split at %d / %d\n",
                    cut_a_1, ind_a->get_chromosome_length());
                LOG("mating_events", "crossover", "ind_b split at %d / %d\n",
                    cut_b_1, ind_b->get_chromosome_length());
            }

            ind_a->dna_split(cut_a_1, &dna_a_1, &dna_a_2);
            ind_b->dna_split(cut_b_1, &dna_b_1, &dna_b_2);

            ind_a->dna_merge(&dna_a_1, &dna_b_2);
            ind_b->dna_merge(&dna_b_1, &dna_a_2);
        }
        break;
        case CUT_TWO_POINTS_DOUBLE_RANDOM:
        case CUT_TWO_POINTS_SINGLE_RANDOM:
        {
            if (conf->get_bool_config(CONFIG_LOG_MATING))
            {
                LOG("mating_events", "crossover", "ind_a split at (%d - %d) / %d",
                    cut_a_1, cut_a_2, ind_a->get_chromosome_length());
                LOG("mating_events", "crossover", "ind_b split at (%d - %d) / %d",
                    cut_b_1, cut_b_2, ind_b->get_chromosome_length());
            }

            ind_a->dna_split(cut_a_1, &dna_a_1, &dna_a_2, cut_a_2, &dna_a_3);
            ind_b->dna_split(cut_b_1, &dna_b_1, &dna_b_2, cut_b_2, &dna_b_3);

            ind_a->dna_merge(&dna_a_1, &dna_b_2, &dna_a_3);
            ind_b->dna_merge(&dna_b_1, &dna_a_2, &dna_b_3);
        }
        break;
    }

    if (conf->get_bool_config(CONFIG_LOG_MATING))
    {
        LOG("mating_events", "crossover", "%s\n", ind_a->info().c_str());
        LOG("mating_events", "crossover", "%s\n", ind_b->info().c_str());
    }
}

void population::create_mating_pool()
{
    if (!pool->size())
        return;

    uint32 mating_num = uint32(pool->size() * conf->get_float_config(CONFIG_MATING_FRACTION));

    INFO("debug", "  selecting %d individual(s) for mating using ", mating_num);
    if (conf->get_bool_config(CONFIG_LOG_MATING))
        LOG_STATIC("mating_events", "mating", "create_mating_pool\n");

    mating_pool.clear();

    switch (conf->get_int_config(CONFIG_MATING_SELECT_TYPE))
    {
        case ROULETTE_WHEEL:
            {
                INFO("debug", "roulette wheel\n");
                roulette_wheel(mating_pool, mating_num);
            }
            break;
        case STOCASTIC_UNIVERSAL:
            {
                INFO("debug", "stocastic universal\n");
                // sort();
                stocastic_universal(mating_pool, mating_num);
            }   
            break;
        case SELECT_BEST:
            {
                INFO("debug", "select best ones\n");
                select_best(mating_pool, mating_num);
            }
            break;
    }
}

void population::transfer()
{  
    if (!pool->size())
        return;

    if (conf->get_float_config(CONFIG_MATING_FRACTION) == 1.0f)
        return;

    uint32 transfer_num = uint32(pool->size()) - uint32(float(pool->size()) * conf->get_float_config(CONFIG_MATING_FRACTION));

    INFO("verbose", "  copying %d individual(s) using ", transfer_num);

    individual_id_list id_pool;    

    switch (conf->get_int_config(CONFIG_TRANSFER_SELECT_TYPE))
    {
        case ROULETTE_WHEEL:
        {
            INFO("debug", "roulette wheel\n");
            if (conf->get_bool_config(CONFIG_ALWAYS_TRANSFER_THE_BEST))
            {
                select_best(id_pool, 1);
                transfer_num--;
            }
            if (transfer_num)
                roulette_wheel(id_pool, transfer_num);
        }
        break;
        case STOCASTIC_UNIVERSAL:
        {
            INFO("debug", "stocastic universal\n");
            if (conf->get_bool_config(CONFIG_ALWAYS_TRANSFER_THE_BEST))
            {
                select_best(id_pool, 1);
                transfer_num--;
            }
            if (transfer_num)
            {
                sort();
                stocastic_universal(id_pool, transfer_num);
            }
        }
        break;
        case SELECT_BEST:
        {
            INFO("debug", "select best ones\n");
            select_best(id_pool, transfer_num);
        }
        break;
    }

    if (!temp_pool)
        temp_pool = new individual_map;

    temp_pool->clear();

    for (individual_id_list::iterator itr = id_pool.begin(); itr != id_pool.end(); ++itr)
    {
        individual *ind_cloned = new individual(*(*pool->find(*itr)).second);
        temp_pool->insert(temp_pool->end(), individual_pair(temp_pool->size(), ind_cloned));
    }
}

void population::roulette_wheel(individual_id_list& id_pool, uint32 number)
{
    weight_map m_weight_map;
    uint32 total_weight = 0;
    float back_fitness = get_worst_individual() ? get_worst_individual()->get_fitness() : 0.0f;
    back_fitness = back_fitness < 0.0f ? -back_fitness : 0.0f;

    for (individual_map::const_iterator itr = pool->begin(); itr != pool->end(); ++itr)
    {
        float fitness = (*itr).second->get_fitness();

        fitness += back_fitness;

        normalize(fitness);

        uint32 u_fitness = uint32(fitness * 1000);

        total_weight += u_fitness;
        m_weight_map[(*itr).first] = u_fitness;
    }

    if (!m_weight_map.size())
        return;

    uint32 selected_weight;
    uint32 weight;
    weight_map::const_iterator itr;

    for (uint32 i = 0; i < number; i++)
    {
         selected_weight = randmm(0, total_weight);
         weight = 0;

         for (itr = m_weight_map.begin(); itr != m_weight_map.end(); ++itr)
         {
             weight += itr->second;
             if (selected_weight <= weight)
             {
                 id_pool.push_back(itr->first);
                 break;
             }
         }
    }
}

void population::stocastic_universal(individual_id_list& id_pool, uint32 number)
{
    weight_map m_weight_map;
    uint32 total_weight = 0;
    float back_fitness = get_worst_individual() ? get_worst_individual()->get_fitness() : 0.0f;
    back_fitness = back_fitness < 0.0f ? -back_fitness : 0.0f;

    for (individual_map::const_iterator itr = pool->begin(); itr != pool->end(); ++itr)
    {
        float fitness = (*itr).second->get_fitness();

        fitness += back_fitness;

        normalize(fitness);

        uint32 u_fitness = uint32(fitness * 1000);

        total_weight += u_fitness;
        m_weight_map[(*itr).first] = u_fitness;
    }

    if (!m_weight_map.size())
        return;

    uint32 selected_weight = randmm(0, total_weight / number);
    uint32 weight = 0;
    weight_map::const_iterator itr = m_weight_map.begin();
    for (uint32 i = 0; i < number; i++)
    {        
         for (; itr != m_weight_map.end(); ++itr)
         {
             weight += itr->second;
             if (selected_weight <= weight)
             {
                 id_pool.push_back(itr->first);
                 break;
             }
         }

         selected_weight += total_weight / number;
    }
}

void population::select_best(individual_id_list& id_pool, uint32 number)
{   
    typedef std::pair<uint32, individual*> best_pair;
    std::list<best_pair> best_map;
    
    for (individual_map::const_iterator itr = pool->begin(); itr != pool->end(); ++itr)
    {
        for (std::list<best_pair>::iterator itr2 = best_map.begin();; ++itr2)
        {  
            if (itr2 == best_map.end() || *((*itr2).second) < *((*itr).second))
            {        
                if (itr2 == best_map.end() && best_map.size() >= number)
                    break;
                
                best_map.insert(itr2, best_pair((*itr).first, (*itr).second));
                
                if (best_map.size() > number)
                {   
                    itr2 = best_map.end();
                    best_map.erase(--itr2);
                }
                break;
            }
        }
    }

    for (std::list<best_pair>::iterator itr2 = best_map.begin(); itr2 != best_map.end(); ++itr2)
        id_pool.push_back((*itr2).first);
}

void population::normalize(float& fitness)
{
    if (conf->get_bool_config(CONFIG_NORMALIZED_FITNESS))
    {        
        float worst_fitness = get_worst_individual() ? get_worst_individual()->get_fitness() : 0.0f;
        worst_fitness = worst_fitness < 0.0f ? -worst_fitness : 0.0f;
        float best_fitness = get_best_individual() ? get_best_individual()->get_fitness() : 0.0f;
        best_fitness += worst_fitness;

        if (best_fitness - worst_fitness != 0.0f)
            fitness = (fitness - worst_fitness) / (best_fitness - worst_fitness);
    }
}

void population::sort()
{
    if (!pool->size())
        return;

    typedef std::pair<individual*, float> best_pair;
    std::list<best_pair> best_map;

    for (individual_map::const_iterator itr = pool->begin(); itr != pool->end(); ++itr)
    {
        for (std::list<best_pair>::iterator itr2 = best_map.begin();; ++itr2)
        {  
            if (itr2 == best_map.end() || (*itr2).second < (*itr).second->get_fitness())
            {
                best_map.insert(itr2, best_pair((*itr).second, (*itr).second->get_fitness()));
                break;
            }
        }
    }

    individual_map* order_pool = new individual_map;
    for (std::list<best_pair>::iterator itr2 = best_map.begin(); itr2 != best_map.end(); ++itr2)
        order_pool->insert(order_pool->end(), individual_pair(order_pool->size(), (*itr2).first));

    delete pool;
    pool = order_pool;
}

void population::mate_individuals()
{
    if (!temp_pool)
        temp_pool = new individual_map;
    individual *ind_a_cloned, *ind_b_cloned;

    switch (conf->get_int_config(CONFIG_CUT_TYPE))
    {
        case CUT_DOUBLE_RANDOM:
            INFO("verbose", "double random\n");
        break;
        case CUT_SINGLE_RANDOM:
            INFO("verbose", "double random\n");
        break;
        case CUT_HALF:
            INFO("verbose", "cut half\n");
        break;
        case CUT_END_SINGLE_RANDOM:
            INFO("verbose", "cut end single random\n");
        break;
        case CUT_END_DOUBLE_RANDOM:
            INFO("verbose", "cut end double random\n");
        break;
        case CUT_TWO_POINTS_DOUBLE_RANDOM:
            INFO("verbose", "two cuts double random\n");
        break;
        case CUT_TWO_POINTS_SINGLE_RANDOM:
            INFO("verbose", "two cuts single random\n");
        break;
    }

    create_mating_pool();

    if (mating_pool.empty())
    {
        if (conf->get_bool_config(CONFIG_LOG_MATING))
            LOG_STATIC("events", "mating", "mating_pool is empty\n");
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
        if (randmm(0,100) <= (conf->get_float_config(CONFIG_MATING_RATE) * 100))
            crossover(ind_a_cloned, ind_b_cloned);

        // Dopo il crossover ci potrebbero essere delle mutazioni
        ind_a_cloned->dna_mutate(mutation_rate);
        ind_a_cloned->dna_mutate(mutation_rate);

        // Aggiungo i figli alla nuova map
        temp_pool->insert(temp_pool->end(),
                 individual_pair(temp_pool->size(), ind_a_cloned));
        temp_pool->insert(temp_pool->end(),
                 individual_pair(temp_pool->size(), ind_b_cloned));

        index +=2; 
    }

    // Se ne rimane uno che non ha trovato moglie lo muto e lo copio nella nuova map
    if (mating_pool.size() == (index + 1))
    {
        ind_a_cloned = new individual(*(*pool->find(*itr)).second);
        ind_a_cloned->dna_mutate(mutation_rate);
        temp_pool->insert(temp_pool->end(),
            individual_pair(temp_pool->size(), ind_a_cloned));
    }

    // Elimino la vecchia generazione e la sostituisco con la nuova
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

float population::get_mutation_rate() const
{
    return mutation_rate;
}

void population::fattest_individuals_shrink()
{
    individual_map::const_iterator itr = pool->begin();

    for (; itr != pool->end(); ++itr)
    {
         while((*itr).second->get_chromosome_length() >= conf->get_int_config(CONFIG_CHROMOSOME_MAX_LENGTH))
            (*itr).second->dna_shrink();
    }
}

uint32  population::size() const
{
    return pool->size();
}

uint32  population::tested_size() const
{
    individual_map::const_iterator itr;
    uint32 count = 0;

    for (itr = pool->begin(); itr != pool->end(); ++itr)
    {
        if ((*itr).second->is_tested())
            count++;
    }

    return count;
}

void population::print(individual *ind) const
{
    if (ind)
        INFO("verbose", "%s\n", ind->info(false).c_str());
}

void population::log(uint32 generation) const
{
    LOG_PTR->set_count("generations", generation);
    LOG_STATIC("generations", "generation", to_string().c_str());
}

int population::load_log(string filename)
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
        INFO("debug","file: %s\n", filename.c_str());
        perror("open");
        return 0;
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
        if (strncmp(p_buffer, "dna", 3) == 0)
        {
            p_buffer = strchr(p_buffer, '\n');
            p_buffer++;
        }
        else
           continue;

        while (p_buffer[0] == '0' || p_buffer[0] == '1' || p_buffer[0] == '\n' )
        {
            temp_dna += p_buffer[0];
            p_buffer++;
        }
        
        if (temp_dna != "")
        {
            while (temp_dna.at(temp_dna.length()-1)  == '\n')
                temp_dna.resize(temp_dna.length()-1);

            ind = new individual(GetStrRowSize(temp_dna), GetStrColSize(temp_dna));
            ind->set_dna(temp_dna);
            pool->insert(pool->end(),
                        individual_pair(created, ind));
        }

        created++;
    }
    delete[] buffer;

    int generation = 0;
    sscanf(basename((char*)filename.c_str()), "generation%d.log", &generation);

    return generation;
}

string population::to_string() const
{
    individual_map::const_iterator itr;
    uint32 count = 0;
    stringstream str;

    for (itr = pool->begin(); itr != pool->end(); ++itr)
    {
        count++;
        str << "individual: #" << count << endl;
        str << (*itr).second->info(false) << endl;
    }

    return str.str();
}

individual* population::get_next_individual()
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
    getlock_pbar();
    p_bar.step();
    releaselock_pbar();
}
