#!/bin/bash
. ~/bin/FunPool.sh
 
date 

echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"
echo -e "\033[32m+ Name    : ReSubmitJobs.sh                                           +\033[0m"
echo -e "\033[32m+ Version : 1.0                                                       +\033[0m"
echo -e "\033[32m+ Author  : Long Zhou                                                 +\033[0m"
echo -e "\033[32m+ Time    : 2014/10/21                                                +\033[0m" 
echo -e "\033[32m+ Describe: This macro only used to resubmit idle jobs                +\033[0m"
echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"

echo `pwd`

if [ ! -e ReRun ]
then 
    mkdir ReRun
#else 
#    rm -f ReRun/*
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
else 
    LOG_ERROR "Wrong input files! "
fi 

if [ -e "rerun.list" ]
then 
    rm rerun.list
else 
    touch rerun.list
fi 

#V1:Only look at the size of error file, is this file is not empty, then this job must be bad jobs, but some times we still have some jobs have error information, but it is not real bad job, so we need set a threshold cut to identify which one is bad. "threshold" set depend on the error information --> see Version 2

#V2:Set a threshold of the size of error files to identify which job is bad job.
for ifile in `find ${logdir}/ -size +30k`
do 
    if [ -s $ifile ]
    then
	LOG_INFO "Found ${count}th bad jobs."
	runid=`basename $ifile .err  | awk -F '_' '{print $2}'`
	session=`basename $ifile .err | awk -F '_' '{print $1}'`
	runsession=`echo "${session}.session.xml"`
	cat list/sched${session}_${runid}.list >> rerun.list

	outfile="rerun_$runid.sh"

	echo -n "star-submit -r " > $outfile
	echo $runid $runsession >> $outfile

	#shuai suggest ,I can do this:
	#echo ",$runid" $runsession >> $outfile

	#Step 4: Re-Submit Idle Jobs
	#bash $outfile

	mv $outfile ReRun/script
	let "count+=1"
    fi 
done 

mv rerun.list ReRun/

if [ ! -e ReRun/log ]
then
    mkdir ReRun/log 
else 
    rm -f ReRun/log/*
fi

echo -e "\033[32mClear OutPut files..\033[0m"
if [ ! -e ReRun/Out ]
then
    mkdir ReRun/Out 
else 
    rm -f ReRun/Out/*
fi 
echo -e "\033[32mClear submit info..\033[0m"
if [ ! -d ReRun/csh ]
then 
    mkdir ReRun/csh 
else 
    rm -f ReRun/csh/*
fi 

if [ ! -d ReRun/list ]
then 
    mkdir ReRun/list 
else 
    rm -f ReRun/list/*
fi 

if [ ! -d ReRun/report ]
then 
    mkdir ReRun/report 
else 
    rm -f ReRun/report/*
fi

echo -e "\033[32mClearning old .xml files... \033[0m"
#rm -f *.session.xml
echo -e "\033[32mClearning old dataset files... \033[0m"
#rm -f *.dataset
#rm -f *.dataset.tmp
echo -e "\033[32mClearning old package and zip... \033[0m"
#rm -f LocalLibraries.zip
#rm -rf LocalLibraries.package/

echo -e "\033[32mClearence work Done ! \033[0m"
echo -e "\033[32m===============================\033[0m"
echo -e "\033[32mStart Submit jobs ....\033[0m"

if [ $issubmit -eq 0 ]
then 
    star-submit submit_rerun.xml
fi 

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
