#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<string>
#include <stdio.h>
#include <stdlib.h>


#include "simulation.h"

using namespace std;

config *conf = new config;

int main(int argc, char ** argv)
{
    int clocks = 30;
    int inputs = 0;

    uint32 tot,det;

    srand(time(NULL));

    if (argc > 1)
        clocks = atoi(argv[1]);

    simulation s;

    init_config();
    conf->conf_filename = "../../config.conf";
    load_config();
    
    inputs = read_n_inputs();

    if (!inputs)
    {
        cout << "error reading inputs " << inputs << endl;
        exit(0);
    }
    else
        cout << "inputs: " << inputs << endl;

    bitmatrix b1(inputs, clocks);
    
    init_env();

    b1.RandomizeAll();
    b1.Print();
    
    cout << "test_simulation:" << endl;
    
    s.execute(b1.ToString(),0);
    
    cout << endl;

    s.get_results(0,tot,det);

    return 0;
}
