#!/bin/sh

cp gentest gentest_tmp
make clean
mv gentest_tmp gentest

rm -Rf *~ *.o

rm -Rf autom4te.cache/ stamp-h1 config.status install-sh \
       aclocal.m4 config.log configure config.status config.h \
       config.h.in depcomp missing Makefile Makefile.in INSTALL
       
rm -Rf src/.deps src/bitmatrix/.deps src/common/.deps
rm -Rf src/.dirstamp src/bitmatrix/.dirstamp src/common/.dirstamp
rm -Rf src/ga/.dirstamp src/simulation/.dirstamp
