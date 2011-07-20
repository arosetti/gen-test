#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<string>

#include "bitmatrix.h"

using namespace std;

int main()
{
    bitmatrix b1(3,3),b2(3,3);
    string s;
    
    srand(time(NULL));
    
    b1.Set(0,0);
    b1.Unset(0,1);
    b1.Set(0,2);
    
    b1.Unset(1,0);
    b1.Set(2,0);
    
    b2.UnsetAll();
    b2.SetRow(b1,0);
    b2.SetCol(b1,0);
    
    b2.Set(2,2);
    
    b2.Print();
    
    printf("\n");
    
    b2.FlipAll();
    b2.Print();
    
    printf("\n");
        
    b2.RandomizeAll();
    b2.RandomizeAll();
    b2.Print();
    
    printf("\n");
    
    s = b2.ToString();
    printf("%s\n",s.c_str());
    
    return 0;
}
