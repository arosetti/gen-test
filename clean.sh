#!/bin/sh

cp gentest gentest_tmp

if [ -e "Makefile" ] ; then
    make clean
fi

mv gentest_tmp gentest

find -name *~ -exec rm -f {} \;
find -name *.o -exec rm -f {} \;

rm -Rf autom4te.cache/ stamp-h1 config.status install-sh \
       aclocal.m4 config.log configure config.status config.h \
       config.h.in depcomp missing Makefile Makefile.in INSTALL \
       src/.deps src/bitmatrix/.deps src/common/.deps src/.dirstamp \
       src/bitmatrix/.dirstamp src/common/.dirstamp src/ga/.dirstamp \
       src/simulation/.dirstamp src/ga/.deps src/simulation/.deps

