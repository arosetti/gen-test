#include <string>
#include <algorithm>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

#ifndef UTILITY_H
#define UTILITY_H

#include "defines.h"
#include "typedefs.h"
#include "logger/logger.h"
#include "rand/rand.h"

#define CMAXSIZE 256

bool   file_exists(string filename);
bool   dir_exists(string dirname);
void   addslash(string &str);
char*  read_file(const char *filename);

uint32 get_columns();

int    exec_command(const char *fmt, ...);

#endif
