#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

#define LOG_PTR() logger::get_instance()
#define LOG(_file, _log, _append) logger::get_instance()->log(_file, _log, _append)

using namespace std;

class logger
{
    private:
    
    static logger* l_singleton;
    
    logger() { };

    public:

    static logger* get_instance()
    { 
        l_singleton = new logger();
        return l_singleton;
    };

    bool log(string file, string log, bool append);
};

#endif
