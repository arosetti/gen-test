#include <cstdio>
#include "ga_engine.h"
#include "config.h"

using namespace std;

int main()
{
    ga_engine ga(N);
    config c;
    
    cout << "gentest: Gentetic Algorithm implementation used to generate test patterns for fpga-like circuits" << endl;

    load_config(&c);

    ga.init(&c);
    ga.evolve();

    return 0;
}
