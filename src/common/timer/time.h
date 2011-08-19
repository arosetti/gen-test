#ifndef TIME_H
#define TIME_H

#include <sys/time.h>
#include <time.h>
#include <sstream>
#include <string>

#include "../typedefs.h"

using namespace std;

typedef struct timer
{
    struct timeval start;
    struct timeval stop;
} timer;

void time_start(timer &time);
void time_stop(timer &time);

double time_diff(timer &time);

string time_format(uint32 sec);

#endif
