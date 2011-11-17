#include "rand.h"

#ifdef USE_MERSENNE_TWISTER

#include "MersenneTwister.h"
MTRand mtrand1;

#endif

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

double uniform_deviate ( int seed )
{
    return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}

void init_rand_seed()
{
#ifdef USE_MERSENNE_TWISTER
    // Use the autoseed  
#else
    srand48(time_seed());
#endif    
}

uint32 randmm(uint32 min, uint32 max)
{
#ifdef USE_MERSENNE_TWISTER
    return min + mtrand1.randInt( max - min );  
#else
    return min + drand48() * (( max - min ) + 1);
#endif    
}

double drandmm(double min, double max)
{
#ifdef USE_MERSENNE_TWISTER
    return min + mtrand1.rand( max - min );    
#else
    return min + drand48() * (( max - min ) + 1);
#endif
}

bool randb()
{
#ifdef USE_MERSENNE_TWISTER
    return (mtrand1.rand() >= 0.5f);  
#else
    return (drand48() >= 0.5f);
#endif
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
    #ifdef USE_MERSENNE_TWISTER
        r1 = 2.0 * mtrand1.rand() - 1.0;
        r2 = 2.0 * mtrand1.rand() - 1.0;
    #else
        r1 = 2.0 * drand48() - 1.0;
        r2 = 2.0 * drand48() - 1.0;
    #endif        
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

/*
uint32 u_wheel_random(uint32 min, uint32 max)
{
    max-=min;
    uint32 total = (max*(max+1))/2; // (max*(max+1)-min*(min-1))/2; // Formula di Gauss   
    uint32 selected = randmm(0, total);
    return ceil((-1.0f + sqrt(1.0f + 8.0f * (double)selected)) / 2) + min;
}
*/

uint32 u_wheel_random(uint32 min, uint32 max, double k)
{
    if (min >= max)
        return max;

    max -= min;

    uint32 total = 0;
    for (uint32 i = 0; i <= max; i++)
        total += uint32(pow(double(i), k));
      
    uint32 selected = randmm(0, total);
    total = 0;
    for (uint32 i = 0; i <= max; i++)
    {
        total += uint32(pow(double(i), k));
        if (selected <= total)
            return i + min;
    }
}

double d_exponential_random(double min, double max, double k)
{
    double ny_max = pow(max, k);
    double ny_min = pow(min, k);

    double y_rnd = randmm(ny_min, ny_max);
    double x_rnd = pow(y_rnd, 1/k);

    //return max - x_rnd + min; // mirror
    return x_rnd;
}

uint32 u_exponential_random(uint32 min, uint32 max, double k)
{
    double ny_max = pow(max, k);
    double ny_min = pow(min, k);

    double y_rnd = randmm(ny_min, ny_max);
    uint32 x_rnd = ceil(pow(y_rnd, 1/k));

    //return max - x_rnd + min; // mirror
    return x_rnd;
}
