#!/bin/bash
. ~/bin/FunPool.sh

date
 
function showhelp()
{
    cat<<EOF
=========================================================
How to Use this macro:
Example:
   ./ReSubFailedJobs.sh reportfile
   reportfile : when using scheduler submit a job, scheduler will creat a report file. 
                e.g sched1452E893A766DCC9A84E9BC234BA72BF.report
   When execute this macro, a submit script will be create, you should check is first, 
   and then if everything is ok, run the submit script by youself. 
=========================================================
EOF
}

echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"
echo -e "\033[32m+ Name    : ReRun.sh                                                  +\033[0m"
echo -e "\033[32m+ Version : 1.0                                                       +\033[0m"
echo -e "\033[32m+ Author  : Long Zhou                                                 +\033[0m"
echo -e "\033[32m+ Time    : 2014/10/21                                                +\033[0m" 
echo -e "\033[32m+ Describe: This macro only used to Re-Submit faild jobs              +\033[0m"
echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"

#  WE WILL USEING REPORT FILE TO IDENTIFY THE FAILED JOBS WHEN SUBMIT TO CLUSTER
echo `pwd`

if [ ! -e ReRun ]
then 
    mkdir ReRun
fi 

report_file=$1			# Report file name and path
current_time=`GetTimeStamp`
schname=`basename $report_file .report`
runid=`echo ${schname#sched}`
runsession=`echo "${runid}.session.xml"`

# Get JobID and Status
declare -a myjobid
declare -a jobstatus

count=0
for i in `grep -ri "|" $report_file | grep "$runid" | awk '{print $2}'`
do 
    myjobid[$count]=$i
    let "count+=1"
done 

count=0
for i in `grep -ri "|" $report_file | grep "$runid" | awk '{print $19}'`
do 
    jobstatus[$count]=$i
    let "count+=1"
done 

arrlen=${#myjobid[@]}

# for((i=0;i<arrlen;i++))
# do 
#     echo "$i ${myjobid[$i]}  ${jobstatus[$i]}"
# done 

outscr="rerun_${current_time}.sh"
touch $outscr
echo -n "star-submit -r " >> $outscr
for((i=0;i<arrlen;i++))
do 
if [ "${jobstatus[$i]}" = "NO" ]
then 
    echo "$i ${myjobid[$i]} ${jobstatus[$i]}"
    runid=`echo ${myjobid[$i]} | awk -F '_' '{print $2}'`
    if [ $i -eq 0 ]
    then 
	echo -n "$runid" >> $outscr
    else 
	echo -n ",$runid" >> $outscr
    fi
fi
done 

echo -n " $runsession" >> $outscr