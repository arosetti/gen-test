#ifndef TIME_H
#define TIME_H

#include <sys/time.h>
#include <time.h>

typedef struct timer
{
    struct timeval start;
    struct timeval stop;
} timer;

void time_start(timer &time);
void time_stop(timer &time);

double time_diff(timer &time);

#endif
