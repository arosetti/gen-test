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

    cout << "do you want to change GA params? (y/n) ";
    cin  >> response;
    cout << endl;

    // sistema di input temporaneo
    if (response == "y" || response == "Y") 
    {
        cout << "chromosome_length: ";
        cin  >> value;
        conf.chromosome_length = value;
    
        cout << "gene_length: ";
        cin  >> value;
        conf.gene_length = value;
        
    }
    cout << endl;

    ga.init(&conf);
    ga.evolve();

    return 0;
}
