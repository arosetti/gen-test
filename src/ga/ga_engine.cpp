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

    if (conf->verbose)
        cout << "* init population" << endl;
    pop = new population();

    if (conf->verbose)
        cout << "* selecting " << conf->population_size << " random individuals..." << endl;
    pop->new_random_population();
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

    if (conf->verbose)
        cout << "* the population is going to evolve for " << conf->max_generations << \
            " generations!" << endl << endl;

    while ( generation++ < conf->max_generations )
    {
        time_start(time);
        if (conf->verbose)
        {
            //for(int i = 0; i< (get_columns()/4) ; i++) cout << "-";
            cout << endl << "# generation: " << generation << endl;
        }
        if (conf->verbose && conf->print_population_size)
            cout << "* population size: " << pop->size() << endl;

        /*if (conf->verbose)
            cout << "* resetting faults" << endl;
        pop->reset_faults();*/

        if (conf->verbose)
            cout << "* executing tests on " << conf->thread_slots << " thread(s)" << endl;
        pop->test_population();

        if (conf->verbose)
            cout << "* calculating population fitnesses" << endl;
        pop->eval_fitnesses();

        if (conf->verbose)
            cout << "* checking for ga stall" << endl;
        if (last_best_fitness < pop->get_best_fitness())
        {
            last_best_fitness = pop->get_best_fitness();
            stall = 0;
            pop->set_mutation_rate(conf->mutation_rate);
        }
        else
            stall++;

        if (stall > conf->max_stall)
        {
            if (conf->verbose)
                cout << "  max stall! (" << stall << ") mutation_rate at max for 1 iteration" << endl;
            pop->set_mutation_rate(0.5f); // magic number
            last_best_fitness = 0;
        }
        else if (stall > 0)
            cout << "  stall is at " << stall << endl;

        if (conf->verbose && conf->print_avg_chromosome_length)
            cout << "* average chromosome length: " << pop->get_avg_chromosome_length() << " bit" << endl;

        if (conf->verbose && conf->print_avg_fitness)
            cout << "* average fitness: " << pop->get_avg_fitness() << endl;

        if (conf->verbose && conf->print_best)
        {
            cout << "* best individual info:" << endl;
            pop->print_best();
        }

        if (conf->debug)
        {
            cout << "* logging generation " << generation << " to file" << endl;
            pop->log(generation);
        }

        if (conf->verbose)
            cout << "* transfer best individuals" << endl;
        pop->transfer_bests();

        if (conf->verbose)
            cout << "* mating individuals" << endl;
        pop->mate_individuals();

        if (conf->mutation_length_gene)
        {
            if (conf->verbose)
                cout << "* shrinking fattest individual" << endl;
            pop->fattest_individual_shrink();
        }

        time_stop(time);

        if (conf->verbose)
            cout << "* iteration time: " << time_format(time_diff(time)) << endl;
    }
}
