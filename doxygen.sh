#!/bin/sh

doxygen Doxyfile

cd doxygen-latex
make
cd ..
