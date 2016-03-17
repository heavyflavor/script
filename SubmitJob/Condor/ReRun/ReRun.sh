#!/bin/bash
. ~/bin/FunPool.sh
 
date 

echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"
echo -e "\033[32m+ Name    : ReSubmitJobs.sh                                           +\033[0m"
echo -e "\033[32m+ Version : 1.0                                                       +\033[0m"
echo -e "\033[32m+ Author  : Long Zhou                                                 +\033[0m"
echo -e "\033[32m+ Time    : 2014/10/21                                                +\033[0m" 
echo -e "\033[32m+ Describe: This macro only used to resubmit faild jobs               +\033[0m"
echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"

echo `pwd`

if [ ! -e ReRun ]
then 
    mkdir ReRun
fi 

count=0

if [ $# -eq 0 ]
then 
    issubmit=1
    logdir="log" 
elif [ $# -eq 1 ]
then 
    issubmit=$1
    logdir="log" 
elif [ $# -eq 2 ]
then
    issubmit=$1
    logdir=$2
fi 

if [ -e "rerun.list" ]
then 
    rm rerun.list
else 
    touch rerun.list
fi 

for ifile in `ls ${logdir}/*.err`
do 
    if [ -s $ifile ]
    then
	LOG_INFO "Found ${count}th bad jobs."
	runid=`basename $ifile .err`
	runname="run_$runid.con"
	cat list/list_${runid}.list >> rerun.list
	cp ./script/$runname ReRun/script
	cp $ifile ReRun/err

	#LOG_INFO "Submiting  ${count}th job... "    
	if [ $issubmit -eq 0 ]
	then 
	    condor_submit script/$runname 
	fi 

	let "count+=1"
    fi 
done 

mv rerun.list ReRun/list

echo -e "\033[32mWork Done !\033[0m"

#Step 5: Remind you when this work done !
#echo -e "\07";
#sleep 1;
#echo -e "\07";
#sleep 1;
#echo -e "\07";
#sleep 1;
#echo -e "\07";
#sleep 1;
#echo $'\07';


#===============================
#Version: 1.0 
#Time: Sat Oct 25 17:37:28 EDT 2014 
#Author: Long Zhou 
#Discribe: This macro used to Re-submit idle jobs 

#===============================
#Version: 1.1 
#Time: Sun Oct 26 22:06:31 EDT 2014 
#Author: Long Zhou 
#Discribe: This version code can re-submit all jobs or only idel jobs 
