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
    float last_best_fitness = 0, step;
    timer time;
    uint32 stall = 0;

    if (!conf)
    {
        INFO("verbose", "GA parameters aren't configured.\n");
        return;
    }

    if (conf->get_int_config(CONFIG_MAX_GENERATIONS))
        INFO("verbose", "* the population is going to evolve for %d generations!\n",
             conf->get_int_config(CONFIG_MAX_GENERATIONS));
    else
        INFO("verbose", "* the population is going to evolve!\n");

    while (!conf->get_int_config(CONFIG_MAX_GENERATIONS) || generation < conf->get_int_config(CONFIG_MAX_GENERATIONS))
    {
        time_start(time);
        generation++;
        INFO("verbose", "\n# generation: %d\n", generation);

        if (conf->get_int_config(CONFIG_POPULATION_SIZE))
            INFO("verbose", "* population size: %d\n", pop->size());

        INFO("verbose", "* executing tests on %d thread(s)\n", conf->get_int_config(CONFIG_THREAD_SLOTS));
        pop->test_population();

        INFO("verbose", "* calculating population fitnesses\n");
        pop->eval_fitnesses();

        switch(conf->get_int_config(CONFIG_MUTATION_TYPE))
        {
            case MUTATION_STATIC:
            break;
            case MUTATION_DYNAMIC:
                //step = (conf->get_float_config(CONFIG_MAX_MUTATION_RATE) - conf->get_float_config(CONFIG_MUTATION_RATE)) / 
                //        (float) (pop->get_best_individual()->GetFaultCoverage() * 100.0f);
                step = conf->get_float_config(CONFIG_MUTATION_RATE) + 
                       (conf->get_float_config(CONFIG_MAX_MUTATION_RATE) - conf->get_float_config(CONFIG_MUTATION_RATE)) *
                       (1.0f - (float) (pop->get_best_individual()->GetFaultCoverage()));
                pop->set_mutation_rate(/*pop->get_mutation_rate() - */step);
                if (pop->get_mutation_rate() < conf->get_float_config(CONFIG_MUTATION_RATE))
                    pop->set_mutation_rate(conf->get_float_config(CONFIG_MUTATION_RATE));
                INFO("verbose", "* mutation rate: %f\n", pop->get_mutation_rate());
            break;
            case MUTATION_STALL_CHECK:
                if (conf->get_int_config(CONFIG_MAX_STALL))
                {
                    INFO("verbose", "* checking stall: %d\n", stall);
                    if (last_best_fitness < pop->get_best_individual()->get_fitness())
                    {
                        last_best_fitness = pop->get_best_individual()->get_fitness();
                        INFO("verbose", "  setting normal mutation rate: %f\n", conf->get_float_config(CONFIG_MUTATION_RATE));
                        pop->set_mutation_rate(conf->get_float_config(CONFIG_MUTATION_RATE));
                        stall = 0;
                    }
                    else
                        stall++;

                    if (stall != 0 && (stall % conf->get_int_config(CONFIG_MAX_STALL)) == 0)
                    {
                        INFO("verbose", "  setting high mutation rate: %f\n", conf->get_float_config(CONFIG_MAX_MUTATION_RATE));
                        pop->set_mutation_rate(conf->get_float_config(CONFIG_MAX_MUTATION_RATE));
                    }
                }
            break;
        }

        if (conf->get_bool_config(CONFIG_PRINT_AVG_CHROMOSOME_LENGTH))
            INFO("verbose", "* average chromosome length: %f bit.\n", pop->get_avg_chromosome_length());

        if (conf->get_bool_config(CONFIG_PRINT_AVG_FAULT_COVERAGE))
            INFO("verbose", "* average fault coverage: %f\n", pop->get_avg_fault_coverage());

        if (conf->get_bool_config(CONFIG_PRINT_AVG_FITNESS))
            INFO("verbose", "* average fitness: %f\n", pop->get_avg_fitness());

        if (conf->get_bool_config(CONFIG_VERBOSE) && conf->get_bool_config(CONFIG_PRINT_BEST))
        {
            INFO("verbose", "* best individual info:\n");
            pop->print((individual*) pop->get_best_individual());

        }
        if (conf->get_bool_config(CONFIG_VERBOSE) && conf->get_bool_config(CONFIG_PRINT_WORST))
        {
            INFO("verbose", "* worst individual info:\n");
            pop->print((individual*) pop->get_worst_individual());
        }

        if (conf->get_bool_config(CONFIG_LOG))
        {
            INFO("verbose", "* logging generation %d to file\n", generation);
            pop->log(generation);

            LOG("events", "best_individual_fitness", "%f", pop->get_best_individual()->get_fitness());
            LOG("events", "best_individual_fault_coverage", "%f", pop->get_best_individual()->GetFaultCoverage());
            LOG("events", "best_individual_chromosome_length", "%d", pop->get_best_individual()->get_chromosome_length());
            LOG("events", "worst_individual_fitness", "%f", pop->get_worst_individual()->get_fitness());
            LOG("events", "worst_individual_fault_coverage", "%f", pop->get_worst_individual()->GetFaultCoverage());
            LOG("events", "worst_individual_chromosome_length", "%d", pop->get_worst_individual()->get_chromosome_length());
            LOG("events", "max_fault_coverage", "%f", pop->get_max_fault_coverage());
            LOG("events", "max_chromosome_length", "%f", pop->get_max_chromosome_length());
            LOG("events", "avg_fault_coverage", "%f", pop->get_avg_fault_coverage());
            LOG("events", "avg_fitness", "%f", pop->get_avg_fitness());
            LOG("events", "avg_chromosome_length", "%f", pop->get_avg_chromosome_length());
            LOG("events", "mutation_rate", "%f", pop->get_mutation_rate());

            if (conf->get_bool_config(CONFIG_GRAPHICS))
                int ret = system("gnuplot graphs.gnuplot > /dev/null 2>&1");
        }

        if (conf->get_bool_config(CONFIG_STOP_AT_100) && 
            pop->get_best_individual()->GetFaultCoverage() == 1.0f)
        {
            INFO("verbose", "all faults have been found!!! stoppig program\n");
            break;
        }

        INFO("verbose",  "* transfer individuals\n");
        pop->transfer();

        INFO("verbose",  "* mating individuals using ");
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
