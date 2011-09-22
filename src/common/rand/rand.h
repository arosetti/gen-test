#include <cstdlib>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>

#ifndef RAND_H
#define RAND_H

#include "../typedefs.h"

void     init_rand_seed();
unsigned time_seed();
uint32   randmm(uint32, uint32);
double   drandmm(double min, double max);
bool     randb();
double   rand_gaussian(double, double);
uint32   u_wheel_random(uint32 min, uint32 max);
uint32   u_cimi_random(uint32 min, uint32 max);
double   d_cimi_random(double min, double max);
#endif
