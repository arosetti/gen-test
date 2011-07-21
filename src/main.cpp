#include <cstdio>
#include <iostream>

#include "ga/ga_engine.h"
#include "../config.h"

using namespace std;

config *conf = new config;

int main()
{
    ga_engine ga;
    string response;
    uint32 value;

    cout << PACKAGE_STRING << ": a gentetic algorithm program ";
    cout << "used to generate test patterns for simulated circuits." << endl;
    cout << endl;

    load_config("config.conf", conf);

    if (conf->interactive) 
    {
        cout << "dna_length: ";
        cin  >> value;
        conf->dna_length = value;
    
        cout << "chromosome_length: ";
        cin  >> value;
        conf->chromosome_length = value;
        
        cout << endl;
    }

    ga.init();
    ga.evolve();

    delete conf;

    return 0;
}
