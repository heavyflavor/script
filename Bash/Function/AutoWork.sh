#!/bin/bash 

date 

if [ $# -eq 1 ]
then 
    version="$1"
fi 

MonitorJobs() 
{
    jobflag=1

    until [ $jobflag -eq 0 ]
    do
	/star/u/zhoulong/script/SubmitJob/CheckMyJobs.sh
	numidle=`condor_q zhoulong | grep running | awk '{print $7}'`
	if [ $numidle -ne 0 ]
	then 
	    /star/u/zhoulong/script/SubmitJob/ReSubmitJobs.sh
	else 
	    echo -e "\033[32mAll jobs finished, \033[0m"
	    jobflag=0
	fi 
    done 
    return 0
}

echo -e "\033[32mSTEP 1 : Compile and Submit Jobs to Computing Cluster \033[0m"

./SubmitJobs.sh

echo -e "\033[32mSTEP 2 : Monitor Jobs Ststus \033[0m"

MonitorJobs

echo -e "\033[32mSTEP 3 : Hadd All Histograms (Submit jobs to computing Clusters)\033[0m"

./hadd.sh 0 

MonitorJobs

./hadd.sh 0 Hist OutData 

MonitorJobs

echo -e "\033[32mSTEP 4 : Analysis OutPut Data .\033[0m"

if [ -e EDstar.root ]
then 
    rm  EDstar.root 
fi 

hadd EDstar.root OutData/*

cd ana

cd ..

echo -e "\033[32mSTEP 5 : Backup Code and Data\033[0m"

bkdir="BK/AnalysisMaker-v${version}-"
mkdir $bkdir
cp -pr ./StRoot/StMyAnalysisMaker/ $bkdir 

