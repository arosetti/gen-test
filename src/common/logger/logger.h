#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdarg.h>
#include <vector>

#include "../typedefs.h"


#define LOG logger::get_instance()
//#define LOG(_file, _log, _append) logger::get_instance()->log(_file, _log, _append)

#define BUF_SIZE 4096

using namespace std;

enum logger_mask
{
    L_APPEND      = 0x00001,
    L_DEBUG       = 0x00002,
    L_VERBOSE     = 0x00004,
    L_PRINT       = 0x00008,
    L_FILE_LOG    = 0x00010,
    L_STATIC      = 0x00020,
    L_INCREMENTAL = 0x00040,
    L_COLOR       = 0x00080,
    L_TIMESTAMP   = 0x00100,
    L_CLOSE       = 0x00200
};

struct logger_profile  // da trasformare in classe
{
    string name, path;
    ofstream ff;
    pthread_mutex_t  l_mutex;

    uint32 l_mask;
    uint32 color, count;

    logger_profile(string l_name, string l_path)
    {
        name = l_name;
        path = l_path;
        count = color = 0;
        l_mask = 0;
        pthread_mutex_init(&l_mutex, NULL);
    }

    void set_opt(uint32 l_m)
    {
        l_mask |= l_m;
    }

    bool get_opt(uint32 l_m)
    {
        return l_mask & l_m;
    }

    void lock()
    {
        pthread_mutex_lock(&l_mutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(&l_mutex);
    }
};

class logger
{
    private:

    static logger*          l_singleton;
    pthread_mutex_t  mutex_log;
    vector<logger_profile*> vct_profiles;

    logger();

    public:

    ~logger();

    static logger* get_instance()
    {
        if(!l_singleton)
            l_singleton = new logger();
        return l_singleton;
    };

    void add_profile(logger_profile *l_profile);
    logger_profile* get_profile(string profile);
    string get_filename(string profile, string fname);

    bool log(string profile, string fname, const char *s_format, ...);
    bool log_static(string profile, string fname, const char *str);

    inline void  getlock()
    {
        pthread_mutex_lock(&mutex_log);
    }

    inline void  releaselock()
    {
        pthread_mutex_unlock(&mutex_log);
    }

};

#endif
