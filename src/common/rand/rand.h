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
bool     randb();
double   rand_gaussian(double, double);
#endif
