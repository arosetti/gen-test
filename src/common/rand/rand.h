#include <cstdlib>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>
#include <iostream>

#ifndef RAND_H
#define RAND_H

#include "../typedefs.h"

// Se definito usa Mersenne Twister
// #define USE_MERSENNE_TWISTER

void     init_rand_seed();
unsigned time_seed();
uint32   randmm(uint32, uint32);
double   drandmm(double min, double max);
bool     randb();
double   rand_gaussian(double, double);
uint32   u_wheel_random(uint32 min, uint32 max, double k = 1);
uint32   u_exponential_random(uint32 min, uint32 max, double k);
double   d_exponential_random(double min, double max, double k);

#endif
