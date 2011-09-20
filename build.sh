#!/bin/sh

if [ $# = 0 ] ; then
    echo -e "usage ./build.sh {[], [release], [profile], [debug], [dist], [clean]}\n"
fi

DOXYGEN=`which doxygen`

if [ -e gentest ] ; then
    rm -f gentest
fi

./autogen.sh

if [ "$1" = "release" ]; then
    ./configure --enable-release
elif [ "$1" = "profile" ]; then
    ./configure --enable-profile
elif [ "$1" = "debug" ]; then
    ./configure --enable-debug
else
    ./configure
fi

count=$(cat /proc/cpuinfo | grep 'model name' | sed -e 's/.*: //' | wc -l)
make -j$count

if [ "$1" = "dist" ] ; then
    make dist-bzip2
fi

if [ "$1" = "clean" ] ; then
    ./clean.sh
fi

if [ "$1" = "release" ] ; then
    strip gentest
fi

echo "* generazione doxygen"
$DOXYGEN Doxyfile > /dev/null 2>&1
