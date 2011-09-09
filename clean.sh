#!/bin/sh

echo "* clean"

if [ -e gentest ] ; then
    cp gentest gentest_tmp
fi

if [ -e "Makefile" ] ; then
    make clean > /dev/null 2>&1
fi

if [ -e gentest_tmp ] ; then
    mv gentest_tmp gentest
fi

rm -f *~

find -name *~ -exec rm -f {} \; > /dev/null 2>&1
find -name *.o -exec rm -f {} \; > /dev/null 2>&1

find -name .dirstamp -exec rm -f {} \; > /dev/null 2>&1
find -name .deps -exec rm -rf {} \; > /dev/null 2>&1

rm -Rf autom4te.cache/ stamp-h1 config.status install-sh \
        config.log aclocal.m4 Makefile Makefile.in configure config.status config.h \
       config.h.in depcomp missing  INSTALL \ > /dev/null 2>&1
rm -f src/bitmatrix/bitmatrix_test src/simulation/simulation_test src/common/logger/logger_test

rm -f logs/* *.log gentest.gdb

