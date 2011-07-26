#include "utility.h"

logger *logger::l_singleton = NULL;


bool file_exists(string filename)
{
    struct stat sts;
    if (stat(filename.c_str(), &sts) == -1 && errno == ENOENT)
        return false;
    return true;
}
