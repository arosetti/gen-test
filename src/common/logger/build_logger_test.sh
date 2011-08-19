#!/bin/bash

rm -f logger.log logger_test
g++ logger.cpp logger_test.cpp -o logger_test
./logger_test
