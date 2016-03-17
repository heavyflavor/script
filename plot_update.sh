#!/bin/bash
date
cd `pwd`

starver DEV
if test $# -lt 1
   then
       #37m --white,30m--black
       echo -e "\033[31m ERROR: You must supply at lest two argument. byebye \033[0m"
       echo -e "\033[32m ERROR: You must supply at lest two argument. byebye \033[0m"
       echo -e "\033[41;37m ERROR: You must supply at lest two argument. byebye \033[0m"
       echo -e "\033[42;37m ERROR: You must supply at lest two argument. byebye \033[0m"
       echo -e "\033[44;37m ERROR: You must supply at lest two argument. byebye \033[0m"
       echo -e "\033[45;37m ERROR: You must supply at lest two argument. byebye \033[0m"
       echo -e "\033[46;37m ERROR: You must supply at lest two argument. byebye \033[0m"
     
       exit 1
fi  

ifile=0
array=(1002 3500 14000 7000 5000 2500)
number=${#array[*]}
echo "We have ${number} internal busy time setting ."

file=$1

runnumber=`echo $file | awk -F '_' '{print $3;}'`
if [ $runnumber = "adc" ];then
   runnumber=`echo $file | awk -F '_' '{print $4;}'`
fi

daynumber=${runnumber:2:3}

echo -e "\033[32m Input data directory is $runnumber \033[0m"
echo -e "\033[32m Day number is $daynumber \033[0m"

if [ ! -f $file ];then
    ln -s out/$file
fi 

echo "Input file is $file"
echo "Use default base pedfile. st_pedestal_adc_15114030_raw_0000001.out.root"
if [ ! -e "st_pedestal_adc_15114030_raw_0000001.out.root" ]
then
   ln -s out/st_pedestal_adc_15114030_raw_0000001.out.root
fi
root -l -b -q 'PedTime.C++("st_pedestal_adc_15114030_raw_0000001.out.root","'$file'",'$daynumber')'
