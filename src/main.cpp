#include <cstdio>
#include <iostream>

#include "ga_engine.h"
#include "../config.h"

using namespace std;

int main()
{
    ga_engine ga;
    config conf;
    string response;
    uint32 value;

    cout << PACKAGE_STRING << ": a gentetic algorithm program ";
    cout << "used to generate test patterns for simulated circuits." << endl;
    cout << endl;

    load_config("config.conf", &conf);

    if (conf.interactive) 
    {
        cout << "dna_length: ";
        cin  >> value;
        conf.dna_length = value;
    
        cout << "chromosome_length: ";
        cin  >> value;
        conf.chromosome_length = value;
        
        cout << endl;
    }

    ga.init(&conf);
    ga.evolve();

    return 0;
}
