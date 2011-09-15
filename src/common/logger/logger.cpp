#include "logger.h"

logger::logger()
{
    pthread_mutex_init(&mutex_log, NULL);
}

logger::~logger()
{
    vector<logger_profile*>::iterator itr;

    getlock();
    for (itr = vct_profiles.begin(); itr != vct_profiles.end(); ++itr)
    {
        (*itr)->ff.close();
        delete (*itr);
    }

    vct_profiles.clear();

    releaselock();
}

void logger::add_profile(logger_profile *l_profile)
{
    if(!l_profile)
        return;

    getlock();
    vct_profiles.push_back(l_profile);
    releaselock();
}

logger_profile* logger::get_profile(string profile)
{
    vector<logger_profile*>::iterator itr;

    getlock();
    for (itr = vct_profiles.begin(); itr != vct_profiles.end(); ++itr)
    {
        if ((*itr)->name == profile)
        {
            releaselock();
            return (*itr);
        }
    }

    releaselock();
    return NULL;
}

string logger::get_filename(string profile, string fname)
{
    string file;
    logger_profile *l_profile = get_profile(profile);

    if(!l_profile)
        return "";

    file  = l_profile->path;
    file += "/";  // addslash da fixare
    file += fname;
    if (l_profile->get_opt(L_INCREMENTAL))
    {
        stringstream str;
        
        str << ++(l_profile->count);
        file += str.str();
    }
    file += ".log";
    return file;
}

bool logger::log(string profile, string fname, const char *fmt, ...)
{
    static char buffer[BSIZE];
    int ret;

    va_list ap;
    va_start(ap, fmt);
    ret = vsnprintf(buffer, BSIZE, fmt, ap);
    va_end(ap);

    if (ret)
        return log_static(profile, fname, buffer);
    else
    {
        perror("vsnprintf");
        return 0;
    }
}


bool logger::log_static(string profile, string fname, const char *str)
{
    logger_profile *l_profile = get_profile(profile);

    if (!l_profile)
        return false;

    if (!l_profile->ff.is_open())
    {
        if (l_profile->get_opt(L_APPEND))
            l_profile->ff.open(get_filename(profile, fname).c_str(), ios::out | ios::app);
        else
            l_profile->ff.open(get_filename(profile, fname).c_str(), ios::out);
    }

    if (l_profile->ff.is_open())
    {
        l_profile->lock();
        l_profile->ff << str << endl;
        if (l_profile->get_opt(L_CLOSE))
            l_profile->ff.close();
        l_profile->unlock();
    }
    else
    {
        perror("logger");
        return true;
    }

    return false;
}

bool logger::info(string profile, const char *fmt, ...)
{
    static char buffer[BSIZE];
    int ret;

    logger_profile *l_profile = get_profile(profile);

    if (!l_profile || !l_profile->get_opt(L_VERBOSE))
       return false;

    va_list ap;
    va_start(ap, fmt);
    ret = vsnprintf(buffer, BSIZE, fmt, ap);
    va_end(ap);

    if (ret)
    {
        l_profile->lock();
        cout << buffer;
        cout.flush();
        l_profile->unlock();
    }
    else
    {
        perror("vsnprintf");
        return 0;
    }
}

