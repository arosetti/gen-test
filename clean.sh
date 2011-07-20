#!/bin/sh

rm -rf *~ *.o

rm -rf autom4te.cache/ stamp-h1 config.status install-sh \
       aclocal.m4 config.log configure config.status config.h \
       config.h.in depcomp missing Makefile Makefile.in INSTALL gentest
       
rm -rf src/.deps src/bitmatrix/.deps src/common/.deps
rm -rf src/.dirstamp src/bitmatrix/.dirstamp src/common/.dirstamp
