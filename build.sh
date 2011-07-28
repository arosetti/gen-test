#!/bin/sh

DOXYGEN=`which doxygen`

if [ -e gentest ] ; then
    rm -f gentest
fi

./autogen.sh
./configure
make

if [ "$1" = "dist" ] ; then
    make dist-bzip2
fi

./clean.sh

if [ "$1" = "strip" ] ; then
    strip gentest
fi

echo "* generazione doxygen"
$DOXYGEN Doxyfile > /dev/null 2>&1

if [ ! -d "logs" ] ; then 
    mkdir "logs"
fi
