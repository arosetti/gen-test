#include "utility.h"

logger *logger::l_singleton = NULL;

bool file_exists(string filename)
{
    struct stat sts;
    if (stat(filename.c_str(), &sts) == -1 && errno == ENOENT)
        return false;
    return true;
}

uint32 get_columns()
{
    char *str = getenv("COLUMNS");
    if (strcmp(str, ""))
        return 0;
    return atoi(str);
}
