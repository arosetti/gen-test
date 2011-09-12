#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>

#define LOG logger::get_instance()
//#define LOG(_file, _log, _append) logger::get_instance()->log(_file, _log, _append)

#define BUF_SIZE 4096

using namespace std;

class logger
{
    private:

    static logger*          l_singleton;
    pthread_mutex_t  mutex_log;

    logger();

    public:

    static logger* get_instance()
    {
        if(!l_singleton)
            l_singleton = new logger();
        return l_singleton;
    };

    bool log(string file, bool append, const char *s_format, ...);

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
