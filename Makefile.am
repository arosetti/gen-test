AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = gentest
gentest_SOURCES = src/ga/ga_engine.h src/ga/ga_engine.cpp   \
                src/ga/population.h src/ga/population.cpp \
                src/ga/individual.h src/ga/individual.cpp \
		src/simulation/simulation.h src/simulation/simulation.cpp \
		src/simulation/tests.h src/simulation/tests.cpp \
        src/simulation/general_tests.h src/simulation/general_tests.cpp \
                src/bitmatrix/bitmatrix.h src/bitmatrix/bitmatrix.cpp \
                src/common/utility.h src/common/utility.cpp \
                src/config.h src/config.cpp \
                src/common/typedefs.h \
                src/common/defines.h \
                src/main.cpp

dist_noinst_SCRIPTS = autogen.sh

