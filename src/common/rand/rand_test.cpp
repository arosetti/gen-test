#include <cstdio>

#include "rand.h"

int main()
{
    int i;
    int tests=3000;
    FILE *ff;
    srand48(time_seed());

    ff = fopen("rand.dat", "w");
    for (i = 0 ; i<tests; i++)
    {
        fprintf(ff, "%f %f\n",(double)drand48(),(double)drand48());
    }
    fclose(ff);

    ff = fopen("gaussian.dat", "w");
    for (i = 0 ; i<tests; i++)
    {
        fprintf(ff, "%f %f\n",(double)rand_gaussian(0,1),(double)rand_gaussian(0,1));
    }
    fclose(ff);

    system("gnuplot rand.gnuplot > /dev/null 2>&1");
}
