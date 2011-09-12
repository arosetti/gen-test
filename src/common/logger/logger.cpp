#include "logger.h"

logger::logger()
{
    pthread_mutex_init(&mutex_log, NULL);
}

bool logger::log(string file, bool append, const char *fmt, ...)
{
    ofstream ff;
    static char buffer[BUF_SIZE];
    int ret;

    va_list ap;
    va_start(ap, fmt);
    ret = vsnprintf(buffer, BUF_SIZE, fmt, ap);
    va_end(ap);

    if (ret)
        return log_static(file, append, buffer);
    else
    {
        perror("vsnprintf");
        return 0;
    }
}

bool logger::log_static(string file, bool append, const char *str)
{
    ofstream ff;

    getlock();

    if (append)
        ff.open(file.c_str(), ios::out | ios::app);
    else
        ff.open(file.c_str(), ios::out);

    if (ff.is_open())
    {
        ff << str << endl;
        ff.close();
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
