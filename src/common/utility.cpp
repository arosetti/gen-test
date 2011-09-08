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

// questo rand utilizza i bit più alti della variabile generata da rand.
// dovrebbero essere migliori.
// source: http://eternallyconfuzzled.com/arts/jsw_art_rand.aspx

double uniform_deviate ( int seed )
{
    return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}

uint32 randmm(uint32 min, uint32 max)
{
    return min + uniform_deviate(rand()) * (( max - min ) + 1);
}

void addslash(string &str)
{
    if (str.c_str()[str.length()] != '/')
        str += "/";
}
