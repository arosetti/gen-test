#!/bin/sh

date=$(date +"%m-%d-%y_%H-%M-%S")
conf_n="gentest.conf"

if [ $# -eq 0 ]; then
    tar="logs-$date.tar.xz"
elif [ $# -eq 1 ]; then
    conf_n=`basename $1`
    tar="$conf_n-$date.tar.xz"
else
    echo "$0 error!"
    exit 1
fi

mkdir -p graphs
echo -e "creating *.eps\n"
gnuplot graphs.gnuplot
echo -e "creating $tar"
cp  "$1" "$conf_n"

tar -cJf $tar graphs/*.eps logs/* $conf_n graphs.gnuplot system.log

if [ "$conf_n" != "gentest.conf" ]; then
    rm -f $conf_n
fi
