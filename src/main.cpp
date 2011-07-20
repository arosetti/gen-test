#include <cstdio>
#include <iostream>
#include "ga_engine.h"
#include "../config.h"

using namespace std;

int main()
{
    ga_engine ga;
    config c;
    string response;
    uint32 value;
    
    cout << PACKAGE_STRING << ": a gentetic algorithm program ";
    cout << "used to generate test patterns for simulated circuits." << endl;
    cout << endl;

    load_config("config.conf", &c);

    cout << "do you want to change GA params? (y/n) ";
    cin  >> response;
    cout << endl;
    
    // sistema di input temporaneo
    if (response == "y" || response == "Y") 
    {
        cout << "chromosome_length: ";
        cin  >> value;
        c.chromosome_length = value;
    
        cout << "gene_length: ";
        cin  >> value;
        c.gene_length = value;
        
    }
    cout << endl;
    
    ga.init(&c);
    ga.evolve();
    
    return 0;
}
