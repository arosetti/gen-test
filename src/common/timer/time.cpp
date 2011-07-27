#include "time.h"

double time_diff(timer &time)
{ 
  struct timeval diff;

  diff.tv_sec  = time.stop.tv_sec - time.start.tv_sec ;
  diff.tv_usec = time.stop.tv_usec - time.start.tv_usec;

  return (double)(diff.tv_sec + (double)diff.tv_usec / (double)CLOCKS_PER_SEC);
}

void time_start(timer &time)
{
    gettimeofday(&time.start,NULL);
}

void time_stop(timer &time)
{
    gettimeofday(&time.stop,NULL);
}
