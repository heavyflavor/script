#!/bin/bash
 
date 

echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"
echo -e "\033[32m+ Name    : ReSubmitJobs.sh                                             +\033[0m"
echo -e "\033[32m+ Version : 1.0                                                       +\033[0m"
echo -e "\033[32m+ Author  : Long Zhou                                                 +\033[0m"
echo -e "\033[32m+ Time    : 2014/10/21                                                +\033[0m" 
echo -e "\033[32m+ Describe: This macro only used to resubmit idle jobs                +\033[0m"
echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"

echo `pwd`

if [ $# -eq 1 ]
then 
    runmode=$1
elif [ $# -eq 0 ]
then 
    runmode=0
else
    echo -e "\033[31mWrong Input Arguments , Stop .\033[0m"
cat <<EOF
User Manual:
script name   : ReSubmitJobs.sh
Input Argument: runmode
Describe:
  runmode=1 , get idle jobs information and resubmit idel jobs, submit script can be found in ReRun/  
  runmode=0 , get idle jobs information but do not re-submit jobs, submit script can be found in ReRun/

Example:
./ReSubmitJobs.sh runmode

EOF
    exit 1
fi  

if [ ! -e ReRun ]
then 
    mkdir ReRun
else 
    rm -f ReRun/*
fi 

#Setp 1: Get Idele job information. 
rerunlist=`condor_q zhoulong -wide | grep zhoulong | awk '{print $9}'`

echo $rerunlist

#Step 2: delete idle jobs 
if [ $runmode -eq 1 ]
then 
    condor_rm zhoulong
fi 
#Step 3: Identifile idle job Id and session name
 
count=0
for runlist in $rerunlist
do 
    outfile="rerun_$count.sh"
    echo -n "star-submit -r " > $outfile
    runid=`echo $runlist | xargs -I aaa basename aaa .csh | awk -F '_' '{print $2}'`
    tempsession=`echo $runlist | xargs -I aaa basename aaa .csh | awk -F '_' '{print $1}'`
    
    schxml=`echo ${tempsession#sched}`

    runsession=`echo "${schxml}.session.xml"`
    echo $runid $runsession >> $outfile

    #Step 4: Re-Submit Idle Jobs
    if [ $runmode -eq 1 ]
    then 
	bash $outfile
    fi 

    mv $outfile ReRun/

    let "count+=1"
done  

echo "Work Done !"

#star-submit submit.xml

