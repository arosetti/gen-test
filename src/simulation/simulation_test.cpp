#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<string>

#include "simulation.h"

using namespace std;

int main()
{
    bitmatrix b1(5,5);
    simulation s;

    if (!s.check_env())
        exit(0);

    b1.RandomizeAll();
    b1.Print();
    
    s.execute();
    
    cout << endl;

    return 0;
}
