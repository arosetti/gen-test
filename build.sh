#!/bin/sh

if [ -e gentest ] ; then
    rm -f gentest
fi

./autogen.sh
./configure
make
./clean.sh

echo "generazione doxygen"
doxygen Doxyfile > /dev/null 2>&1

if [ ! -d "logs" ] ; then 
    mkdir "logs"
fi
