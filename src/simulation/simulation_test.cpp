#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<string>

#include "simulation.h"

using namespace std;

config *conf = new config;

int main()
{
    bitmatrix b1(5,30);
    simulation s;

    load_config("../../config.conf", conf);

    if (!s.check_env())
        exit(0);

    b1.RandomizeAll();
    b1.Print();
    
    s.execute(b1.ToString());
    
    cout << endl;

    return 0;
}
