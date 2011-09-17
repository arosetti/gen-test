#include <cstdio>
#include <iostream>
#include <signal.h>

#include "ga/ga_engine.h"
#include "../config.h"

using namespace std;

config *conf = NULL;
static timer t_gentest;
extern vector<pthread_t> threads_id;

void sigint_callback_handler(int);

int main(int argc, char **argv)
{
    init_rand_seed();
    ga_engine ga;

    INFO("verbose", "%s: a gentetic algorithm program ", PACKAGE_STRING);
    INFO("verbose", "used to generate test patterns for simulated circuits.\n");
    INFO("verbose", "authors: Alessandro Rosetti - Daniele Lazzarini (C) 2011\n\n");

    signal(SIGINT, sigint_callback_handler);

    conf = new config;
    conf->load_args(argc, argv);

    time_start(t_gentest);
    ga.init();
    ga.evolve();

    delete conf;
    delete LOG_PTR;

    return 0;
}

void sigint_callback_handler(int signum)
{
    vector<pthread_t>::iterator itr = threads_id.begin();

    INFO("verbose", "caught SIGINT signal\n");
    INFO("verbose", "* killing simulation threads\n");
    for( ; itr != threads_id.end(); ++itr)
        pthread_kill(*itr, SIGINT);

    clean_env();

    time_stop(t_gentest);
    INFO("verbose", "* program time: %s\n" , time_format(time_diff(t_gentest)).c_str());

    cout.flush();
    exit(signum);
}
