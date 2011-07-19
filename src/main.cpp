#include <cstdio>
#include <iostream>
#include "ga_engine.h"

using namespace std;

int main()
{
    ga_engine ga;
    config c;
    
    cout << "gentest: Gentetic Algorithm implementation used to generate test patterns for fpga-like circuits" << endl << endl;

    load_config("gentest.conf", &c);

    ga.init(&c);
    ga.evolve();
    
    return 0;
}
