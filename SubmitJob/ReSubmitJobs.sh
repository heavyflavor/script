#!/bin/bash
 
date 

echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"
echo -e "\033[32m+ Name    : ReSubmitJobs.sh                                           +\033[0m"
echo -e "\033[32m+ Version : 1.0                                                       +\033[0m"
echo -e "\033[32m+ Author  : Long Zhou                                                 +\033[0m"
echo -e "\033[32m+ Time    : 2014/10/21                                                +\033[0m" 
echo -e "\033[32m+ Describe: This macro only used to resubmit idle jobs                +\033[0m"
echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"

echo `pwd`

if [ $# -eq 1 ]
then 
    runmode=$1
    echo -e "\033[32m Running mode: Re-Submit all jobs( include current running jobs )\033[0m"
elif [ $# -eq 0 ]
then 
    runmode=0
    echo -e "\033[32m Running mode: Only Re-Submit idle jobs .\033[0m"
else
    echo -e "\033[31mWrong Input Arguments , Stop .\033[0m"
cat <<EOF
User Manual:
script name   : ReSubmitJobs.sh
Input Argument: runmode
Describe:
  runmode=1 , get all jobs information and resubmit All jobs, submit script can be found in ReSub/  
  runmode=0 , only get idle jobs information and re-submit jobs, submit script can be found in ReSub/

Example:
./ReSubmitJobs.sh runmode

EOF
    exit 1
fi  

if [ ! -e ReSub ]
then 
    mkdir ReSub
else 
    rm -f ReSub/*
fi 

#Setp 1: Get Idele job information. 

#runinfo=`condor_q zhoulong -wide | grep zhoulong`

if [ $runmode -eq 1 ]
then
    rerunlist=`condor_q zhoulong -wide | grep zhoulong | awk '{print $9}'`
    rerunidlist=`condor_q zhoulong -wide | grep zhoulong | awk '{print $1}'`
    echo $rerunlist 
    echo $rerunidlist
    #condor_rm zhoulong
else 
    rerunlist=`condor_q zhoulong -wide | grep zhoulong | grep " I " | awk '{print $9}'`
    rerunidlist=`condor_q zhoulong -wide | grep zhoulong | grep " I " | awk '{print $1}'`
    echo $rerunlist 
    echo $rerunidlist
fi 

#Step 2: Identifile job Id and session name
 
count=0

for runlist in $rerunlist
do 
    outfile="rerun_$count.sh"
    echo -n "star-submit -r " > $outfile

    runid=`echo $runlist | xargs -I aaa basename aaa .csh | awk -F '_' '{print $2}'`
    tempsession=`echo $runlist | xargs -I aaa basename aaa .csh | awk -F '_' '{print $1}'`
    
    schxml=`echo ${tempsession#sched}`

    runsession=`echo "${schxml}.session.xml"`
    if [ ! -e $runsession ]
    then 
	echo -e "\033[31mYou are in wrong place, Please go to right directory. and try a gain ! Bye-Bye \033[0m"
	exit 1 
    fi 
    echo $runid $runsession >> $outfile

    #shuai suggest ,I can do this:
    #echo ",$runid" $runsession >> $outfile
    let "count+=1"
done  

#Step 3: delete identified jobs 
for rerunid in $rerunidlist
do 
    condor_rm $rerunid
done 

#Step 4: Re-Submit Idle Jobs
for ijob in `ls rerun_*`
do
    bash $ijob
    mv $ijob ReSub/
done 
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

#===============================
#Version: 1.2 
#Time: Thu Jun 18 08:46:55 PDT 2015 
#Author: Long Zhou 
#Discribe: Added some protection, if we call this command in wrong place, the script will not delete the idle or error jobs untill you are in right place. 

#===============================
#Version: 1.3 
#Time: Thu Dec  3 10:50:38 PST 2015 
#Author: Long Zhou 
#Discribe: Change the directory ReRun to ReSub  
