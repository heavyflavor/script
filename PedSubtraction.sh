#!/bin/bash

cd `pwd`

starver DEV
if test $# -lt 2
   then
       #37m --white,30m--black
       echo -e "\033[31m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[32m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[41;37m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[42;37m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[44;37m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[45;37m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[46;37m ERROR: You must supply two argument. byebye \033[0m"
     
       exit 1
fi 

file=`basename $1 .sstQa.root`
pedfile=$2

outdir1=`echo $1 | awk -F '_' '{print $3}'`
datatype=`echo $pedfile | awk -F '_' '{print $3}'`
if [ $datatype = "adc" ];then
   outdir2=`echo $pedfile | awk -F '_' '{print $4}'`
else 
   outdir2=`echo $pedfile | awk -F '_' '{print $3}'`
fi

outdir="${outdir1}_${outdir2}"

echo "input file is ${file}"
echo "input pedestal file is $pedfile"
echo "Out Dir is ${outdir}"

if [ ! -d "$outdir" ];then
   mkdir "$outdir"
   root -l -b -q 'PedSubtraction.C++("'$file'","'$pedfile'","'$outdir'",1,1)'
elif [ $# -eq 4 ];then
   root -l -b -q 'PedSubtraction.C++("'$file'","'$pedfile'","'$outdir'",'$3','$4')'
else
   root -l -b -q 'PedSubtraction.C++("'$file'","'$pedfile'","'$outdir'",0,1)'
fi



