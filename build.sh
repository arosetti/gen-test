#!/bin/sh

./autogen.sh
./configure
make
./clean.sh
echo "generazione doxygen"
doxygen Doxyfile > /dev/null 2>&1
