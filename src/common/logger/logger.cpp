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
    cout << "profile not found!" << endl;

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
    static char buffer[BUF_SIZE];
    int ret;

    va_list ap;
    va_start(ap, fmt);
    ret = vsnprintf(buffer, BUF_SIZE, fmt, ap);
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
    string file;

    if (!l_profile)
        return false;

    if (l_profile->get_opt(L_PRINT))
    {
        // colori e timestamps
        cout << str << endl;
    }

    if (!l_profile->get_opt(L_FILE_LOG))
        return true;

    file = get_filename(profile, fname);

    if (!l_profile->ff.is_open())
    {
        if (l_profile->get_opt(L_APPEND))
            l_profile->ff.open(file.c_str(), ios::out | ios::app);
        else
            l_profile->ff.open(file.c_str(), ios::out);
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
        cout << file << endl;
        perror("logger");
        releaselock();
        return true;
    }

    releaselock();

    return false;
}
