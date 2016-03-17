#!/bin/bash

date 

if [ $# -ne 1 ]
then
   echo -e "\033[31mERROR:Please Input one argument and try again !  Byebye. \033[0m"
   exit 1
fi 
infile=$1

filename=`basename $infile .txt`
srdo=${filename##*s_}  #sst_pedestals_s1_r1 --->s1_r1

echo -e "\033[32mSector and Rdo is $srdo \033[0m"

case $srdo in 
  s1_r1) 
     rdo=0
     sector=1;;
  s1_r2) 
     rdo=1
     sector=1;;
  s1_r3) 
     rdo=2
     sector=1;;
  s2_r1)
     rdo=3
     sector=2;;
  s2_r2) rdo=4;;
  *) echo -e "\033[31mWrong Sector and Rdo number : $srdo \033[0m"
     exit 1;;
esac

echo -e "\033[32mInPut file is $infile \033[0m"
echo -e "\033[32mSector: $sector \033[0m"
echo -e "\033[32mRDO   : $rdo \033[0m"
echo -e "\033[32mOutPut file is Pedestal_rdo${rdo}.txt \033[0m"

if [ ! -e out ];then
   mkdir out 
fi 

./readtext $infile $rdo
