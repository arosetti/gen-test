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

    cout << PACKAGE_STRING << ": a gentetic algorithm program ";
    cout << "used to generate test patterns for simulated circuits." << endl;
    cout << "authors: Alessandro Rosetti - Daniele Lazzarini (C) 2011" << endl;
    cout << endl;

    signal(SIGINT, sigint_callback_handler);
    //signal(SIGKILL, sigint_callback_handler);

    conf = new config;
    conf->load_args(argc, argv);

    time_start(t_gentest);
    ga.init();
    ga.evolve();
    while(1);
    return 0;
}

void sigint_callback_handler(int signum)
{
    int ret;
    vector<pthread_t>::iterator itr = threads_id.begin();
    string cmd;

    INFO("verbose", "\n\n* caught SIGINT signal\n");

while(1);

    INFO("verbose", "* killing simulation threads\n");
    for( ; itr != threads_id.end(); ++itr)
        pthread_cancel(*itr);

    INFO("verbose", "* killing simulators\n");  // TODO creare i segnali dei thread
    exec_command("killall %s > /dev/null 2>&1", 
                 conf->get_string_config(CONFIG_SIMULATOR_BIN).c_str());

    clean_env();

    time_stop(t_gentest);
    INFO("verbose", "* program time: %s\n" , time_format(time_diff(t_gentest)).c_str());

    cout.flush();
    delete conf;
    delete LOG_PTR;

    exit(signum);
}
