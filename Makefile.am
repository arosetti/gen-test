AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = gentest
gentest_SOURCES = src/ga_engine.h src/ga_engine.cpp   \
                src/population.h src/population.cpp \
                src/individual.h src/individual.cpp \
                src/bitmatrix/bitmatrix.h src/bitmatrix/bitmatrix.cpp \
                src/common/utility.h src/common/utility.cpp \
                src/config.h src/config.cpp \
                src/common/typedefs.h \
                src/common/defines.h \
                src/main.cpp

dist_noinst_SCRIPTS = autogen.sh

