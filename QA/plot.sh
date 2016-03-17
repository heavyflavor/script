#!/bin/bash

cd `pwd`

starver DEV
##set logfile = `basename $1 .Ped.root`;
date
if [ ! -d pic ];then
   mkdir pic
fi
for input in `cat txtlist`
do
  logfile=`basename $input .out.root`
  echo $logfile
if [ ! -d "$logfile" ];then
   mkdir $logfile
fi
 root -l -b -q 'plots.C("'${logfile}'","'${logfile}'")' > ${logfile}/plots.log
done
