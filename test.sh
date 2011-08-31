#!/bin/bash
# TODO test valgrind o test gentest

if [ -e gentest ] ; then
time ./gentest > gentest.log
fi
