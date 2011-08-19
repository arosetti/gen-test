#include <cstdio>
#include <iostream>

#include "ga/ga_engine.h"
#include "../config.h"

using namespace std;

config *conf = new config;

int main(int argc, char **argv)
{
    srand(time(NULL));

    ga_engine ga;

    cout << PACKAGE_STRING << ": a gentetic algorithm program ";
    cout << "used to generate test patterns for simulated circuits." << endl;
    cout << endl;

    init_config();
    load_config();
    load_args(argc, argv);

    ga.init();
    ga.evolve();

    delete conf;
    delete LOG_PTR();

    return 0;
}
