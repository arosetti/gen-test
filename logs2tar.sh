#!/bin/sh

date=$(date +"%m-%d-%y_%H-%M-%S")
tar="logs-$date.tar.xz"

echo -e "creating *.eps\n"
gnuplot gnuplot.conf
echo -e "creating $tar"
tar -cJf $tar *eps logs/* config.conf gnuplot.conf
