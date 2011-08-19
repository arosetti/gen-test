#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<string>

#include "bitmatrix.h"

using namespace std;

int main()
{
    bitmatrix b1(5,5),b2(5,5);
    string s("1,1\n1,1,1,1\n0,0");

    srand(time(NULL));
/*
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
*/
    b2.RandomizeAll();
    b1.RandomizeAll();

    printf("b2\n");
    b2.Print();
    printf("\n\n");

    printf("b1\n");
    b1.Print();
    printf("\n\n");
/*
    b2.Import(b1);
    b2.Print();
    printf("\n");

    b2.SetCol(s, 0);

    printf("%s\n",s.c_str());
    printf("\n");
    b2.Print();
    printf("\n");


    b1.RandomizeAll();
    b1.Print();
    printf("\n");
    b1.Resize(6,4);
    b1.UnsetRow(0);
    b1.UnsetRow(1);
    b1.Print();
    printf("\n");
    b1.Import(s);
    b1.Print();
    printf("\n");
*/
    string s1 =  b1.GetRow(1);
    printf("%s\n\n",s1.c_str());

    s = b2.ToString();
    printf("%s\n",s.c_str());

    s1 =  b2.GetCols(1,4);
    printf("%s\n\n",s1.c_str());

    b1.SetCols(s1,1);
    b1.Print();

    printf("\nb1 cols: %d\n", (int) GetStrColSize(b1.ToString()));
    printf("b1 rows: %d\n", (int) GetStrRowSize(b1.ToString()));

    //b1.Resize(5,30);

    printf("\nb1\n");
    string t = b1.ToString();
    printf("%s\n",t.c_str());
    StrTranspose(t);
    printf("b1 trasposta: \n%s\n\n", t.c_str());
    
    b1.AttachCols(b2.ToString());
    b1.Print();
   
    return 0;
}
