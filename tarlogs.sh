#!/bin/sh

date=$(date +"%m-%d-%y_%H-%M-%S")
tar="logs-$date.tar.xz"

mkdir -p graphs
echo -e "creating *.eps\n"
gnuplot gnuplot.conf
echo -e "creating $tar"
tar -cJf $tar graphs/*.eps logs/* gentest.conf graphs.gnuplot system.log
