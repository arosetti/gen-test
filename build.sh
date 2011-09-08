#!/bin/sh

if [ $# = 0 ] ; then
    echo -e "usage ./build.sh {[clean] || [strip] || [dist] || [run] || [run_debug]}\n"
fi

DOXYGEN=`which doxygen`

if [ -e gentest ] ; then
    rm -f gentest
fi

./autogen.sh
./configure

make -j5

if [ "$1" = "dist" ] ; then
    make dist-bzip2
fi

if [ "$1" = "clean" ] ; then
./clean.sh
fi

if [ "$1" = "strip" ] ; then
    strip gentest
fi

echo "* generazione doxygen"
$DOXYGEN Doxyfile > /dev/null 2>&1

if [ ! -d "logs" ] ; then 
    mkdir "logs"
fi

if [ "$1" = "run" ] ; then
    ./gentest
fi

if [ "$1" = "debug" ] ; then
    ./debug
fi

