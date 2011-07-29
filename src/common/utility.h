#include <string>
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
#include "../config.h"

extern config *conf;

bool file_exists(string filename);
int  parse_options(int argc, char **argv);

#endif
