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
        cout << "iterations: ";
        cin  >> value;
        conf->max_iterations = value;
       
        cout << "chromosome_num: ";
        cin  >> value;
        conf->chromosome_num = value;
    
        cout << "chromosome_start_len: ";
        cin  >> value;
        conf->chromosome_start_len = value;
        
        cout << endl;
    }

    ga.init();
    ga.evolve();

    delete conf;

    return 0;
}
