#include "ga_engine.h"

ga_engine::ga_engine()
{
    generation = 0;
    srand(time(NULL));
}

ga_engine::~ga_engine()
{
    delete pop;
}

void ga_engine::init()
{
    if (conf->verbose)
        cout << "* init population" << endl;
    pop = new population();

    if (conf->verbose)
        cout << "* selecting " << conf->population_size << " random individuals..." << endl;
    pop->new_random_population();
}

void ga_engine::evolve()
{
    float avg_fitness = 0;
    timer time;

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
            cout << endl << "* generation: " << generation << endl;
        }
        if (conf->verbose && conf->print_population_size)
            cout << "* population size: " << pop->size() << endl;

        /*if (conf->verbose)
            cout << "* reset faults" << endl;
        pop->reset_faults();*/

        if (conf->verbose)
            cout << "* executing tests" << endl;
        pop->test_population();

        if (conf->verbose)
            cout << "* calculating population fitnesses" << endl;
        pop->eval_fitnesses();

        if (conf->verbose && conf->print_avg_fitness)
        {
            avg_fitness = pop->get_avg_fitness();
            cout << "* average fitness: " << avg_fitness << endl;
        }

        if (conf->verbose && conf->print_best)
            pop->print_best();

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

        time_stop(time);
         
        if (conf->verbose)
            cout << "* iteration time: " << time_format(time_diff(time)) << endl;
    }
}
