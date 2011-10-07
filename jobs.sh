#!/bin/bash

JOBDIR="jobs"

echoc () { 
  echo -e "\E["$1"m$2\E[0m";
}

if [ $# -eq 1 ]; then
    echoc 31 "using \"$1\" as jobdir"
    JOBDIR=$1
elif [ $# -gt 1 ]; then
    echoc 31 "usage: $0 <jobdir>"
    exit 1
fi

if [ ! -d $JOBDIR ];then
    echoc 31 "jobdir \"$JOBDIR\" does not exists!"
    exit 1;
fi

vjobs=$(ls -f $JOBDIR/*.conf)

count=0
for j in $vjobs ; do
    count=`expr $count + 1`
done

njobs=$count
count=0
echoc 31 "njobs: $njobs\n\n$vjobs\n"

for j in $vjobs ; do
    echoc 32 "--------------------------------------------";
    echoc 31 ">> running job \"$j\". ($count/$njobs)";
    ./gentest -c $j #debugger?? TODO da passare i parametri internamente a gdb
    count=`expr $count + 1`
    ./tarlogs.sh $j
    echoc 31 "moving log to \"$JOBDIR\""
    mv *conf*.tar.* $JOBDIR > /dev/null
done


