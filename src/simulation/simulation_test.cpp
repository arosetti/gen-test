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
    
    if (argc > 1)
        clocks = atoi(argv[1]);
        
    bitmatrix b1(5,clocks);
    simulation s;

    load_config("../../config.conf", conf);

    //if (!s.init_env())
    //    exit(0);

    b1.RandomizeAll();
    b1.Print();
    
    cout << "test_simulation:" << endl;
    
    s.execute(b1.ToString());
    
    cout << endl;

    return 0;
}
