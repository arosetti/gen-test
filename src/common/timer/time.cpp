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

string time_format(uint32 sec)
{
    stringstream out;

    uint32 sec_res, min, hours, hours_res, days, days_res, years, years_res;

    if (sec < 60)
        out << sec << " second(s)"; 
    else if (sec < 3600)
    {
        min = (uint32)sec/60;
        sec_res = (uint32)sec%60;
        out << min << " minute(s), " << sec_res << " second(s)"; 
    }
    else if ( sec < 86400 )
    {
        hours = (uint32) sec/3600;
        hours_res = (uint32)sec%3600;
        min = (uint32)hours_res/60;
        sec_res = (uint32)sec%60;
        out << hours << " hour(s), " << min << " minute(s), " << sec << " second(s)"; 
    }
    else if ( sec < 31536000 )
    {
        days = (uint32) sec/86400;
        days_res = (uint32)sec%86400;
        hours = (uint32) days_res/3600;
        hours_res = (uint32)(sec - 86400)%3600;
        min = (uint32)hours_res/60;
        sec= (uint32)sec%60;
        
        out << days<<" day(s), "<<hours<< " hour(s), "<< min<< " minute(s), "<<sec<< " second(s)"; 
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
        sec= (uint32)sec%60;
        out << years << " year(s), " << days << " day(s), " << hours << " hour(s), " << min << " minute(s), " << sec << " second(s)"; 
    }

    return out.str();
}

