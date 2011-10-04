#!/bin/bash

JOBDIR="jobs"

echoc () { 
  echo -e "\E["$1"m$2\E[0m";
}

mkdir -p jobs

count=0
v_jobs=$(ls -f jobs/*.conf)
echoc 31 "${#v_jobs[*]} jobs found." 
echoc 31 "$v_jobs"

for j in $v_jobs ; do
    echoc 32 "--------------------------------";
    echoc 32 ">> $j $count of ${#v_jobs[*]}";
    ./gentest -c $j
    count++;
done
cd - > /dev/null

mv log*.tar.bz2 jobs


