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

string time_format(float sec)
{
    stringstream out;

    float sec_res;
    uint32 min, hours, hours_res, days, days_res, years, years_res;

    if ( sec != 0.0f )
        sec_res = fmod(sec, (float)60);

    if ( sec == 0.0f )
        out << sec << " second(s)"; 
    else if (sec < 60)
        out << sec << " second(s)"; 
    else if (sec < 3600)
    {
        min = (uint32)sec/60;
        out << min << " minute(s), " << sec_res << " second(s)"; 
    }
    else if ( sec < 86400 )
    {
        hours = (uint32) sec/3600;
        hours_res = (uint32)sec%3600;
        min = (uint32)hours_res/60;
        out << hours << " hour(s), " << min << " minute(s), " << sec_res << " second(s)"; 
    }
    else if ( sec < 31536000 )
    {
        days = (uint32) sec/86400;
        days_res = (uint32)sec%86400;
        hours = (uint32) days_res/3600;
        hours_res = (uint32)(sec - 86400)%3600;
        min = (uint32)hours_res/60;
        out << days<<" day(s), "<< hours << " hour(s), "<< min << " minute(s), " << sec_res << " second(s)"; 
    }
    else
    {
        years = (uint32) sec/31536000;
        years_res = (uint32) sec%31536000;
        days = (uint32) years_res/86400;
        days_res = (uint32)sec%86400;
        hours = (uint32) days_res/3600;
        hours_res = (uint32)(sec - 86400)%3600;
        min = (uint32)hours_res/60;
        out << years << " year(s), " << days << " day(s), " << hours << " hour(s), " << min << " minute(s), " << sec_res << " second(s)"; 
    }

    return out.str();
}

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

