#include <cstdio>
#include "ga_engine.h"

using namespace std;

int main()
{
    ga_engine ga(N);
    
    cout << "gentest: Gentetic Algorithm implementation used to generate test patterns for fpga-like circuits" << endl;

    read_config();

    setup_simulation();

    ga.init();
    ga.evolve();

    return 0;
}
