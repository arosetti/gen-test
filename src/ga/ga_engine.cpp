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

    INFO("verbose", "* init population\n");
    pop = new population();

    if (conf->get_string_config(CONFIG_LOAD_LOG_FILENAME) != "")
    {
        INFO("verbose", "* loading log %s ...\n", conf->get_string_config(CONFIG_LOAD_LOG_FILENAME).c_str());
        generation = pop->load_log(conf->get_string_config(CONFIG_LOAD_LOG_FILENAME));
    }
    else
    {
        INFO("verbose", "* selecting %d random individuals\n", conf->get_int_config(CONFIG_POPULATION_SIZE));
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
        INFO("verbose", "GA parameters aren't configured.\n");
        return;
    }

    INFO("verbose", "* the population is going to evolve for %d generations!\n", conf->get_int_config(CONFIG_MAX_GENERATIONS));

    while ( generation++ < conf->get_int_config(CONFIG_MAX_GENERATIONS))
    {
        time_start(time);
        INFO("verbose", "\n# generation: %d\n");

        if (conf->get_int_config(CONFIG_POPULATION_SIZE))
            INFO("verbose", "* population size: %d\n", pop->size());

        /*if (conf->verbose && conf->read_faults_file)
            INFO("verbose", "* resetting faults\n");
        pop->reset_faults();*/

        INFO("verbose", "* executing tests on %d thread(s)\n", conf->get_int_config(CONFIG_THREAD_SLOTS));
        pop->test_population();

        INFO("verbose", "* calculating population fitnesses\n");
        pop->eval_fitnesses();

        if (conf->get_int_config(CONFIG_MAX_STALL))
        {
            INFO("verbose", "* checking stall: %d\n");
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
                INFO("verbose", "  setting high mutation rate for 1 iteration\n");
                pop->set_mutation_rate(conf->get_float_config(CONFIG_MUTATION_STALL_RATE));
            }
        }

        if (conf->get_bool_config(CONFIG_PRINT_AVG_CHROMOSOME_LENGTH))
            INFO("verbose", "* average chromosome length: %d bit.\n", pop->get_avg_chromosome_length());

        if (conf->get_bool_config(CONFIG_PRINT_AVG_FITNESS))
            INFO("verbose", "* average fitness: %f\n", pop->get_avg_fitness());

        if (conf->get_bool_config(CONFIG_VERBOSE) && conf->get_bool_config(CONFIG_PRINT_BEST))
        {
            INFO("verbose", "* best individual info:\n");
            pop->print_best();
        }

        if (conf->get_bool_config(CONFIG_LOG))
        {
            INFO("verbose", "* logging generation %d to file\n");
            pop->log(generation);

            LOG("events", "best_individual_fitness", "%f", pop->get_best_fitness());
            LOG("events", "best_individual_fault_coverage", "%f", pop->get_best_individual()->get_fault_coverage());
            LOG("events", "best_individual_chromosome_length", "%d", pop->get_best_individual()->get_chromosome_length());
            LOG("events", "best_fault_coverage", "%f", pop->get_best_fault_coverage());
            LOG("events", "max_chromosome_length", "%f", pop->get_best_chromosome_length());

            if (conf->get_bool_config(CONFIG_GRAPHICS))
                int ret = system("gnuplot gnuplot.conf > /dev/null 2>&1");
        }

        if (conf->get_bool_config(CONFIG_STOP_AT_100) &&  pop->get_best_fault_coverage() == 100.f)
        {
            INFO("verbose", "100% faults found!!! stoppig program\n");
            break;
        }

        INFO("verbose",  "* transfer best individuals\n");
        pop->transfer_bests();

        INFO("verbose",  "* mating individuals\n");
        pop->mate_individuals();

        if (conf->get_bool_config(CONFIG_MUTATION_LENGTH_GENE))
        {
            INFO("verbose", "* shrinking fattest individuals\n");
            pop->fattest_individuals_shrink();
        }

        time_stop(time);

        INFO("verbose",  "* iteration time: %s\n", time_format(time_diff(time)).c_str());
    }
}
