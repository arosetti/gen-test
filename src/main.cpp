#include <cstdio>
#include <iostream>
#include <signal.h>

#include "ga/ga_engine.h"
#include "../config.h"

using namespace std;

config *conf = new config;

void sigint_callback_handler(int);

int main(int argc, char **argv)
{
    srand(time_seed());

    ga_engine ga;

    cout << PACKAGE_STRING << ": a gentetic algorithm program ";
    cout << "used to generate test patterns for simulated circuits." << endl;
    cout << "authors: Alessandro Rosetti - Daniele Lazzarini (C) 2011" << endl;
    cout << endl;


    signal(SIGINT,sigint_callback_handler);

    init_config();
    load_config();
    load_args(argc, argv);
    check_config();

    ga.init();
    ga.evolve();

    delete conf;
    delete LOG_PTR();

    return 0;
}

void sigint_callback_handler(int signum)
{
   cout << endl << "caught SIGINT signal " << endl;
   // kill dei thread per evitare segfault
   clean_env();
   exit(signum);
}
