#include <cstdio>
#include <iostream>
#include "ga_engine.h"

using namespace std;

int main()
{
    ga_engine ga;
    config c;
    string response;
    uint32 value;
    
    cout << "gentest: Gentetic Algorithm implementation used to generate test patterns for fpga-like circuits" << endl << endl;

    load_config("gentest.conf", &c);

    cout << "do you want to change GA params? (y/n) ";
    cin  >> response;
    cout << endl;
    
    // sistema di input temporaneo
    if(response == "y") 
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
