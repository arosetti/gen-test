#include <cstdio>

#include "rand.h"
/* mean m, standard deviation s */

/*
float gauss(float m, float s)
{
    float x1, x2, w, y1;
    static float y2;
    static int use_last = 0;

    if (use_last)
    {
        y1 = y2;
        use_last = 0;
    }
    else
    {
        do {
            x1 = 2.0 * drand48() - 1.0;
            x2 = 2.0 * drand48() - 1.0;
            w = x1 * x1 + x2 * x2;
        } while ( w >= 1.0 );

        w = sqrt( (-2.0 * log( w ) ) / w );
        y1 = x1 * w;
        y2 = x2 * w;
        use_last = 1;
    }

    return( m + y1 * s );
}
*/

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

    // il random non va bene
    for (i = 0 ; i<1500; i++)
    {
        printf("%d ", randmm(0,1));
    }
}
