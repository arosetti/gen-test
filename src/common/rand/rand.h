#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

#ifndef RAND_H
#define RAND_H

#include "../typedefs.h"

unsigned time_seed();
uint32   randmm(uint32, uint32);
bool     randb();
void     init_rand_seed();

#endif
