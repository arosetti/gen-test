#!/bin/sh

g++ rand_test.cpp rand.cpp -o rand_test -lm
./rand_test
rm -f *.dat
rm -f rand_test.o
