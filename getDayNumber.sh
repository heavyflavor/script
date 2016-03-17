#!/bin/bash

#date 

#echo `pwd`

infile=$1

if [ ! -f $infile ]
then
  echo "Can not found this file"
else 
  runnumber=`ls $infile | awk -F '/' '{print $7;}'`

  #echo "Runnumber is $runnumber"

  tempname=${runnumber#15}

  #echo "temp name is $tempname"

  daynumber=${tempname%[0-9][0-9][0-9]}

  #echo "Day number is $daynumber"

  echo "${runnumber}/${daynumber}"

fi 
