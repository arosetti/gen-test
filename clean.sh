#!/bin/sh

if [ -e gentest ] ; then
    cp gentest gentest_tmp
fi

if [ -e "Makefile" ] ; then
    make clean
fi

if [ -e gentest_tmp ] ; then
    mv gentest_tmp gentest
fi

find -name *~ -exec rm -f {} \;
find -name *.o -exec rm -f {} \;

find -name .dirstamp -exec rm -f {} \;
find -name .deps -exec rm -rf {} \;

rm -Rf autom4te.cache/ stamp-h1 config.status install-sh \
       aclocal.m4 config.log configure config.status config.h \
       config.h.in depcomp missing Makefile Makefile.in INSTALL \

