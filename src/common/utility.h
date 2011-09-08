#include <string>
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

bool file_exists(string filename);
uint32 get_columns();
uint32 randmm(uint32, uint32);
void addslash(string &str);

#endif
