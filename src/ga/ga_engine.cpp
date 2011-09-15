#include "ga_engine.h"

ga_engine::ga_engine()
{
    generation = 0;
}

ga_engine::~ga_engine()
{
    delete pop;
}

void ga_engine::init()
{
    init_env();

    if (conf->get_bool_config(CONFIG_VERBOSE))
        cout << "* init population" << endl;
    pop = new population();

    if (conf->get_string_config(CONFIG_LOAD_LOG_FILENAME) != "")
    {
        if (conf->get_bool_config(CONFIG_VERBOSE))
            cout << "* loading log " << conf->get_string_config(CONFIG_LOAD_LOG_FILENAME) << " ..." << endl;
        generation = pop->load_log(conf->get_string_config(CONFIG_LOAD_LOG_FILENAME));
    }
    else
    {
        if (conf->get_bool_config(CONFIG_VERBOSE))
            cout << "* selecting " << conf->get_int_config(CONFIG_POPULATION_SIZE) << " random individuals..." << endl;
        pop->new_random_population();
    }
}

void ga_engine::evolve()
{
    float last_best_fitness = 0;
    timer time;
    uint32 stall = 0;

    if (!conf)
    {
        cout << "GA parameters aren't configured." << endl;
        return;
    }

    if (conf->get_bool_config(CONFIG_VERBOSE))
        cout << "* the population is going to evolve for " << conf->get_int_config(CONFIG_MAX_GENERATIONS) << \
            " generations!" << endl << endl;

    while ( generation++ < conf->get_int_config(CONFIG_MAX_GENERATIONS))
    {
        time_start(time);
        if (conf->get_bool_config(CONFIG_VERBOSE))
            cout << endl << "# generation: " << generation << endl;

        if (conf->get_bool_config(CONFIG_VERBOSE) && conf->get_int_config(CONFIG_POPULATION_SIZE))
            cout << "* population size: " << pop->size() << endl;

        /*if (conf->verbose && conf->read_faults_file)
            cout << "* resetting faults" << endl;
        pop->reset_faults();*/

        if (conf->get_bool_config(CONFIG_VERBOSE))
            cout << "* executing tests on " << conf->get_int_config(CONFIG_THREAD_SLOTS) << " thread(s)" << endl;
        pop->test_population();

        if (conf->get_bool_config(CONFIG_VERBOSE))
            cout << "* calculating population fitnesses" << endl;
        pop->eval_fitnesses();

        if (conf->get_int_config(CONFIG_MAX_STALL))
        {
            if (conf->get_bool_config(CONFIG_VERBOSE))
                cout << "* checking stall: " << stall << endl;
            if (last_best_fitness < pop->get_best_fitness())
            {
                last_best_fitness = pop->get_best_fitness();
                pop->set_mutation_rate(conf->get_float_config(CONFIG_MUTATION_RATE));
                stall = 0;
            }
            else
                stall++;

            if (stall != 0 && (stall % conf->get_int_config(CONFIG_MAX_STALL)) == 0)
            {
                if (conf->get_bool_config(CONFIG_VERBOSE))
                    cout << "  setting high mutation rate for 1 iteration" << endl;
                pop->set_mutation_rate(conf->get_float_config(CONFIG_MUTATION_STALL_RATE));
            }
        }

        if (conf->get_bool_config(CONFIG_VERBOSE) && conf->get_bool_config(CONFIG_PRINT_AVG_CHROMOSOME_LENGTH))
            cout << "* average chromosome length: " << pop->get_avg_chromosome_length() << " bit" << endl;

        if (conf->get_bool_config(CONFIG_VERBOSE) && conf->get_bool_config(CONFIG_PRINT_AVG_FITNESS))
            cout << "* average fitness: " << pop->get_avg_fitness() << endl;

        if (conf->get_bool_config(CONFIG_VERBOSE) && conf->get_bool_config(CONFIG_PRINT_BEST))
        {
            cout << "* best individual info:" << endl;
            pop->print_best();
        }

        if (conf->get_bool_config(CONFIG_LOG))
        {
            cout << "* logging generation " << generation << " to file" << endl;
            pop->log(generation);

            LOG("events", "best_individual_fitness", "%f", pop->get_best_fitness());
            LOG("events", "best_individual_fault_coverage", "%f", pop->get_best_individual()->get_fault_coverage());
            LOG("events", "best_individual_chromosome_length", "%d", pop->get_best_individual()->get_chromosome_length());
            LOG("events", "best_fault_coverage", "%f", pop->get_best_fault_coverage());
            LOG("events", "max_chromosome_length", "%f", pop->get_best_chromosome_length());

            if (conf->get_bool_config(CONFIG_GRAPHICS))
                int ret = system("gnuplot gnuplot.conf > /dev/null 2>&1");
        }

        if (conf->get_bool_config(CONFIG_VERBOSE))
            cout << "* transfer best individuals" << endl;
        pop->transfer_bests();

        if (conf->get_bool_config(CONFIG_VERBOSE))
            cout << "* mating individuals" << endl;
        pop->mate_individuals();

        if (conf->get_bool_config(CONFIG_MUTATION_LENGTH_GENE))
        {
            if (conf->get_bool_config(CONFIG_VERBOSE))
                cout << "* shrinking fattest individuals" << endl;
            pop->fattest_individuals_shrink();
        }

        time_stop(time);

        if (conf->get_bool_config(CONFIG_VERBOSE))
            cout << "* iteration time: " << time_format(time_diff(time)) << endl;
    }
}
