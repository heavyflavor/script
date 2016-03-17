#!/bin/bash

date 

if [ $# -ne 1 ]
then
   echo -e "\033[31mERROR:Please Input one argument and try again !  Byebye. \033[0m"
   exit 1
fi 
infile=$1

sector=`basename $infile | awk -F '_' '{print $4;}'`
srdo=`basename $infile .txt | awk -F '_' '{print $5;}'`
if [ $sector = "s1" ]
then
   if [ $srdo = "r1" ];then
      rdo=0
   elif [ $srdo = "r2" ];then
      rdo=1
   else
      rdo=2
   fi  
else
   if [ $srdo = "r1" ];then
      rdo=3 
   else
      rdo=4
   fi 
fi

echo -e "\033[32mInPut file is $infile \033[0m"
echo -e "\033[32mSector: $sector \033[0m"
echo -e "\033[32mRDO   : $srdo \033[0m"
echo -e "\033[32mOutPut file is Pedestal_rdo${rdo}.txt \033[0m"

if [ ! -e out ];then
   mkdir out 
fi 

./readtext $infile $rdo
