#!/bin/bash
. ~/bin/FunPool.sh
 
date 

echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"
echo -e "\033[32m+ Name    : ArchiveJobs.sh                                            +\033[0m"
echo -e "\033[32m+ Version : 1.0                                                       +\033[0m"
echo -e "\033[32m+ Author  : Long Zhou                                                 +\033[0m"
echo -e "\033[32m+ Time    : 2015/07/21                                                +\033[0m" 
echo -e "\033[32m+ Describe: This macro only used to save job informations.            +\033[0m"
echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"

echo `pwd`

if [ $# -eq 1 ]
then 
    runmode=$1
    echo -e "\033[32m Running mode: Archive all jobs( include current running jobs )\033[0m"
elif [ $# -eq 0 ]
then 
    runmode=0
    echo -e "\033[32m Running mode: Only Archive idle jobs .\033[0m"
else
    echo -e "\033[31mWrong Input Arguments , Stop .\033[0m"
cat <<EOF
User Manual:
This macro used to archive jobs infromation, for change rcf node purpose. when you found you jobs running too slow, you can archive all job information, and change node. resume your jobs.  

script name   : ArchiveJobs.sh
Input Argument: runmode
Describe:
  runmode=1 , get all jobs information and resubmit All jobs, submit script can be found in ReRun/  
  runmode=0 , only get idle jobs information and re-submit jobs, submit script can be found in ReRun/

Example:
./ReSubmitJobs.sh runmode

After run this script, you will got a shell script[defaults name is ResumeJobs.sh], that script will records all jobs id, session name. if you want , you can re-submit to rcf. 
just do like this : 
./ResumeJobs.sh

EOF
    exit 1
fi  

if [ ! -e ArchiveJobs ]
then 
    mkdir ArchiveJobs
else 
    rm -f ArchiveJobs/*
fi 

LOG_INFO "Setp 1: Get Idele job information. "

#runinfo=`condor_q zhoulong -wide | grep zhoulong`

if [ $runmode -eq 1 ]
then
    rerunlist=`condor_q zhoulong -wide | grep zhoulong | awk '{print $9}'`
    rerunidlist=`condor_q zhoulong -wide | grep zhoulong | awk '{print $1}'`
    # echo $rerunlist > /dev/null
    # echo $rerunidlist > /dev/null
else 
    rerunlist=`condor_q zhoulong -wide | grep zhoulong | grep " I " | awk '{print $9}'`
    rerunidlist=`condor_q zhoulong -wide | grep zhoulong | grep " I " | awk '{print $1}'`
    # echo $rerunlist > /dev/null
    # echo $rerunidlist > /dev/null
fi 

LOG_INFO "Step 2: Identifile job Id and session name"

outfile="ResumeJobs.sh"
echo -n "star-submit -r " > $outfile

sessionname=""
count=0
for runlist in $rerunlist
do 
    runid=`echo $runlist | xargs -I aaa basename aaa .csh | awk -F '_' '{print $2}'`
    tempsession=`echo $runlist | xargs -I aaa basename aaa .csh | awk -F '_' '{print $1}'`
    
    schxml=`echo ${tempsession#sched}`

    runsession=`echo "${schxml}.session.xml"`
    if [ ! -e $runsession ]
    then 
	echo -e "\033[31mYou are in wrong place, Please go to right directory. and try a gain ! Bye-Bye \033[0m"
	exit 1 
    fi 

    if [ $count -eq 0 ];then 
	echo -n "$runid" >> $outfile
    else 
	echo -n ",$runid" >> $outfile
    fi
    #shuai suggest ,I can do this:
    #echo ",$runid" $runsession >> $outfile

    sessionname="$runsession"
    let "count+=1"
done  
echo -n " $sessionname" >> $outfile

LOG_INFO "Step 3: delete identified jobs "
for rerunid in $rerunidlist
do 
    condor_rm $rerunid
done 

LOG_INFO "Step 4: Archive All informations"
cp ResumeJobs.sh ArchiveJobs/
cp $sessionname ArchiveJobs

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
