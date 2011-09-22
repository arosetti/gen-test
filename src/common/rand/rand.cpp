#include "rand.h"

// questo rand utilizza i bit più alti della variabile generata da rand.
// dovrebbero essere migliori.
// source: http://eternallyconfuzzled.com/arts/jsw_art_rand.aspx

unsigned time_seed()
{
    time_t now = time ( 0 );
    unsigned char *p = (unsigned char *)&now;
    unsigned seed = 0;
    size_t i;

    for ( i = 0; i < sizeof now; i++ )
        seed = seed * ( UCHAR_MAX + 2U ) + p[i];

    return seed;
}

void init_rand_seed()
{
    srand(time_seed());
    srand48(time_seed());
}

double uniform_deviate ( int seed )
{
    return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}

uint32 randmm(uint32 min, uint32 max)
{
    return min + uniform_deviate(rand()) * (( max - min ) + 1);
}

double drandmm(double min, double max)
{
    return min + drand48() * (( max - min ) + 1);
}

bool randb()
{
   return (drand48() >= 0.5f);
}

double rand_gaussian(double eta, double sigma)
{
    static bool cached = false;
    static double cached_value;
    double rsquare, factor, r1, r2;

    if(cached)
    {
        cached = false;
        return cached_value;
    }
    
    do
    {
        r1 = 2.0 * drand48() - 1.0;
        r2 = 2.0 * drand48() - 1.0;
        rsquare = r1*r1 + r2*r2;
    }
    while(rsquare >= 1.0 || rsquare == 0.0);

    double polar = -2.0 * log(rsquare) / rsquare;

    if(polar > 0.0)
        factor = sqrt(polar);
    else
        factor = 0.0;

    cached_value = r1 * factor;
    cached = true;

    return ((r2 * factor) * sigma + eta);
}

uint32 u_wheel_random(uint32 min, uint32 max)
{
    max-=min;
    uint32 total = (max*(max+1))/2; // (max*(max+1)-min*(min-1))/2; // Formula di Gauss   
    uint32 selected = randmm(0, total);
    return ceil((-1.0f + sqrt(1.0f + 8.0f * (double)selected)) / 2) + min;
    /*total = 0;
    for (uint32 i = 0; i <= max; i++)
    {
        total += i;
        if (selected <= total)
            return i + min;
    }*/
}

// Da rivedere
uint32 u_cimi_random(uint32 min, uint32 max)
{
    double temp = pow((double)randmm(min, max), 2);
    temp = temp / pow((double)max+1, 2);
    return uint32 (temp * max)+1;
}

double d_cimi_random(double min, double max)
{
    double temp = pow((double)drandmm(min, max), 2);
    temp = temp / pow((double)max+1, 2);
    return temp * max + 1.0f;
}
