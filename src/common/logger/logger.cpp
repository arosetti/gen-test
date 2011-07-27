#include "logger.h"

bool logger::log(string file, string log, bool append)
{
    ofstream ff;

    if (append)
        ff.open(file.c_str(), ios::out | ios::app);
    else
        ff.open(file.c_str(), ios::out);

    if (ff.is_open())
    {
        ff << log << endl;
        ff.close();
    }
    else
    {
        cout << file << endl;
        perror("logger");
        return true;
    }
    return false;
}
