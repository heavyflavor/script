#!/bin/bash

date 

#Check jobs runing status.
numjobs=`condor_q zhoulong | grep running | awk '{print $9}'`
numidle=`condor_q zhoulong | grep running | awk '{print $7}'`
until [ $numjobs -eq 0 -a $numidle -lt 100 ]
do
    echo -e "\033[32mFound $numjobs running , and $numidle idle\033[0m"
    sleep 180
    numjobs=`condor_q zhoulong | grep running | awk '{print $9}'`
    numidle=`condor_q zhoulong | grep running | awk '{print $7}'`
done

#Remind me when this work is Done !
echo -e "\033[32mAll Jobs are finished ! \033[0m"
echo -e "\07";
sleep 1;
echo -e "\07";
sleep 1;
echo -e "\07";
sleep 1;
echo -e "\07";
sleep 1;
echo $'\07';
 
#Both  echo $'\07' and echo -e "\07" are represent the warning tone.
