#include "utility.h"

logger *logger::l_singleton = NULL;

bool file_exists(string filename)
{
    struct stat sb;
    stat(filename.c_str(), &sb);

    if (errno == ENOENT || (sb.st_mode & S_IFMT) != S_IFREG)
    return false;

    return true;
}

bool dir_exists(string dirname)
{
    struct stat sb;
    stat(dirname.c_str(), &sb);

    if (errno == ENOENT || (sb.st_mode & S_IFMT) != S_IFDIR)
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

void addslash(string &str)
{
     if (str.at(str.length()-1) != '/')
        str += "/";
}
