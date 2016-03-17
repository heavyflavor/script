#!/bin/bash

cd `pwd`

starver DEV
##set logfile = `basename $1 .Ped.root`;
date
if [ ! -d pic ];then
   mkdir pic
fi

ifile=0
array=(1002 3500 14000 7000 5000 2500)
number=${#array[*]}
echo "We have ${number} internal busy time setting ."

for input in `cat txtlist`
do
  logfile=`basename $input .out.root`
  echo $logfile
  runnumber=`echo $input | awk -F '_' '{print $3}'`
  echo " Run number is ${runnumber}"
  if [ ! -d "$logfile" ];then
     mkdir $logfile
  fi 
  root -l -b -q 'plots.C("'${logfile}'",'${runnumber}',"'${array[$ifile]}'")' > ${logfile}/plots.log
  let "ifile+=1"
done
