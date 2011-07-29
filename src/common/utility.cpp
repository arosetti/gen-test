#include "utility.h"

logger *logger::l_singleton = NULL;


bool file_exists(string filename)
{
    struct stat sts;
    if (stat(filename.c_str(), &sts) == -1 && errno == ENOENT)
        return false;
    return true;
}

int parse_options(int argc, char **argv)
{
    int aflag = 0;
    int bflag = 0;
    char *cvalue = NULL;
    int index;
    int opt;

    opterr = 0;

    while ((opt = getopt (argc, argv, "abc:")) != -1)
    switch (opt)
    {
       case 'a':
         aflag = 1;
         break;
       case 'b':
         bflag = 1;
         break;
       case 'c':
         cvalue = optarg;
         break;
       case '?':
         if (optopt == 'c')
           fprintf (stderr, "Option -%c requires an argument.\n", optopt);
         else if (isprint (optopt))
           fprintf (stderr, "Unknown option `-%c'.\n", optopt);
         else
           fprintf (stderr,
                    "Unknown option character `\\x%x'.\n",
                    optopt);
         return 1;
       default:
         abort();
    }

    printf ("aflag = %d, bflag = %d, cvalue = %s\n",
           aflag, bflag, cvalue);

    for (index = optind; index < argc; index++)
     printf ("Non-option argument %s\n", argv[index]);
     
    return 0;
}
