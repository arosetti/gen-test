#!/bin/sh

g++ rand_test.cpp rand.cpp rand.h -o rand_test -lm
./rand_test
rm -f *.dat
rm -f rand_test.o
