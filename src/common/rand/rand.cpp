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

bool randb()
{
   return (drand48() >= 0.5f);
}
