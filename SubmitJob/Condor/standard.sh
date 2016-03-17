#!/bin/bash

cd `pwd`

starver DEV
inputfile=$1
logfile=`basename $inputfile .daq`;

echo "Input file is ${inputfile}"

if [ $# -ne 1 ];then
   echo -e "\033[31m ERROR : You Must Supply Only One Argument. Stop !! \033[0m"
   exit 1
fi

echo $logfile

if [ ! -d log ]
then
   mkdir log 
fi 

if [ -f "log/${logfile}.log" ];then
   rm -f log/${logfile}.log
fi

runnumber=`echo $inputfile | awk -F '_' '{print $3}'`
if [ $runnumber = "adc" ];then
   runnumber=`echo $inputfile | awk -F '_' '{print $4}'`
fi

echo "Run runmber is $runnumber"

if [ ! -d $runnumber ];then
   mkdir $runnumber
fi

#root4star -b -q 'bfc.C(1,500,"in db MakeEvent y2014 ssdDb sst_daq spt Debug","'$1'")' > log/${logfile}.log
root4star -b -q 'bfc.C(1,500,"in db MakeEvent y2014 ssdDb sst_daq spt","'$1'")' > log/${logfile}.log

mv ${logfile}.Ped.root ${runnumber}
mv ${logfile}.sstQa.root ${runnumber}
