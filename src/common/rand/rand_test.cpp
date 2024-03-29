#include <cstdio>
#include "rand.h"

int main()
{
    int i;
    int tests=4000;
    FILE *ff;
    srand48(time_seed());

    ff = fopen("rand.dat", "w");
    for (i = 0 ; i<tests; i++)
    {
        fprintf(ff, "%f %f\n",(double)drandmm(0,1),(double)drandmm(0,1));
    }
    fclose(ff);

    ff = fopen("gaussian.dat", "w");
    for (i = 0 ; i<tests; i++)
    {
        fprintf(ff, "%f %f\n",(double)rand_gaussian(0,1),(double)rand_gaussian(0,1));
    }
    fclose(ff);

    ff = fopen("exp.dat", "w");
    for (i = 0 ; i<tests; i++)
    {
        double r1 = (double) d_exponential_random(0,30.0,1.7f) , r2 =  (double) drandmm(0,20);
        printf("%f %f\n", r1, r2);
        fprintf(ff, "%f %f\n", r1, r2);
    }
    fclose(ff);

    system("gnuplot rand.gnuplot > /dev/null 2>&1");
}
