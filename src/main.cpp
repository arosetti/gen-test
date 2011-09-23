#include <cstdio>
#include <iostream>
#include <signal.h>

#include "ga/ga_engine.h"
#include "../config.h"

using namespace std;

config *conf = NULL;
static timer t_gentest;
//extern vector<pthread_t> threads_id;

void exit(void);
void sigint_handler(int sig);

int main(int argc, char **argv)
{
    init_rand_seed();
    ga_engine ga;

    cout << PACKAGE_STRING << ": a gentetic algorithm program ";
    cout << "used to generate test patterns for simulated circuits." << endl;
    cout << "authors: Alessandro Rosetti - Daniele Lazzarini (C) 2011" << endl;
    cout << endl;

    signal(SIGINT, sigint_handler);
    atexit(exit);

    conf = new config;
    conf->load_args(argc, argv);

    time_start(t_gentest);
    ga.init();
    ga.evolve();

    delete conf;
    delete LOG_PTR;

    return 0;
}

void sigint_handler(int sig)
{
    exit();
    signal(SIGINT, SIG_DFL);
    kill(getpid(), SIGINT); // harakiri
}

void exit(void)
{
    time_stop(t_gentest);
    INFO("verbose", "\n\n* program execution time: %s\n" , time_format(time_diff(t_gentest)).c_str());
}
