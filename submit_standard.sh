#!/bin/bash

date
ifile=0

if [ ! -d script ];then 
   mkdir script
else
   rm -f script/*
fi 
if [ ! -d log ];then
   mkdir log
else
   rm -f log/*
fi
mydir=`pwd`
echo "Current dir is ${mydir}"

  for FILE in `cat runlist`
  do
    echo "Input data is ${FILE}"

    var=`echo $FILE | awk -F '_' '{print $3}'`
    if [ $var = "adc" ];then
       var=`echo $FILE | awk -F '_' '{print $4}'`
    fi 
    echo $var

    cp standard.con run_${ifile}_${var}.con

    echo "Initialdir     = ${mydir}" >> run_${ifile}_${var}.con 
    echo "Executable     = standard.sh" >> run_${ifile}_${var}.con
    echo "Arguments      = $FILE" >> run_${ifile}_${var}.con 
    echo "Output         = log/${ifile}_${var}.out" >> run_${ifile}_${var}.con
    echo "Error          = log/${ifile}_${var}.err" >> run_${ifile}_${var}.con
    echo "Log            = log/${ifile}_${var}.log" >> run_${ifile}_${var}.con
    echo "Queue"         >> run_${ifile}_${var}.con
                           
    condor_submit run_${ifile}_${var}.con 
                                              
    mv run_${ifile}_${var}.con script/
    let "ifile+=1"
done


