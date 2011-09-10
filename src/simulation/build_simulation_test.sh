#!/bin/bash

g++ simulation_test.cpp simulation.cpp simulation_utility.cpp ../bitmatrix/bitmatrix.cpp ../config.cpp ../common/utility.cpp -lconfuse -o simulation_test

