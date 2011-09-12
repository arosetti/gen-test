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
    vsnprintf(buffer, BUF_SIZE, fmt, ap);
    va_end(ap);

    getlock();

    if (append)
        ff.open(file.c_str(), ios::out | ios::app);
    else
        ff.open(file.c_str(), ios::out);

    if (ff.is_open())
    {
        ff << buffer << endl;
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
