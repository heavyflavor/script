#!/bin/bash

date 

echo "\033[32mCompile source code ..\033[0m"

make 

if [ $? -eq 1 ]
then 
    echo -e "\033[31mCompile failed ! Please Check code \033[0m"
    exit 1 
fi 

inDir=/star/institutions/lbl_scratch/jhthomas/ZhouLong/20-May-2014

if [ $# -ne 1 ]
then
    echo -e "\033[31mERROR:Please Input Pedestal Run Number and try again !  Byebye. \033[0m"
    exit 1
fi 

runnumber=$1

echo -e "\033[32mInput Run Number is $runnumber \033[0m"
echo -e "\033[32mStart Creating pedestal file list.... \033[0m"

if [ ! -e pedlist ]
then
    ls $inDir/* | grep -i "$runnumber" > pedlist
else 
    sed -i '1,$d' pedlist
    ls $inDir/* | grep -i "$runnumber" >> pedlist
fi

echo -e "\033[32mPedestal File list :  \033[0m"
cat pedlist 

echo -e "\033[32m================================\033[0m"
echo -e "\033[32mSTART CREATE NEW PEDESTAL FILS \033[0m"
for infile in `cat pedlist`
do
    filename=`basename $infile .txt`
    srdo=${filename##*[0-9][0-9]_}  #sst_pedestals_s1_r1 --->s1_r1

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
	s2_r2) 
	    rdo=4
	    sector=2;;
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

    mystatus="status/Rdo${rdo}.status.root"
    ./readtext $infile $rdo $mystatus
done
echo -e "\033[32mCREATE NEW PEDESTAL FILES DONE  \033[0m" 
echo -e "\033[32m================================\033[0m"

#===============================
#Version: 1.1 
#Time: Sat May 17 23:42:55 EDT 2014 
#Author: Long Zhou 
#Discribe: solved get sector and rdo number problem  
#Work Dir: ~/data02/DEV14/SST/Firmware/NewVersion/Automatic/
