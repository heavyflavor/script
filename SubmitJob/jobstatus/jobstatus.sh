#!/bin/bash 
. ~/bin/FunPool.sh

date 

cd `pwd`

timestamp=`GetTimeStamp`
datafile="jobstatus_$timestamp.dat" 
touch Jobstatus/$datafile

if [ ! -e Jobstatus ]
then 
    mkdir Jobstatus
fi 

if [ ! -e log ]
then 
    LOG_ERROR "No such file or directory [log/]"
    exit 1
fi 

if [ $# -eq 1 ]
then 
    logfile="${1}*.out"
else 
    logfile="*.out"
fi 

for ifile in `ls log/${logfile}`
do 
    #percent=`cat $ifile | grep -ri "process" | sed -n '$p' | awk '{print $2}'`
    percent=`grep -ri "process" $ifile | sed -n '$p' | awk '{print $2}'`

    if [ "${percent}aa" = "aa" ]
    then 
	number=0
    else 
	number=${percent%\%*}
    fi 
    echo "$number" >> Jobstatus/$datafile
done 

# root -l -b -q 'jobstatus.C("jobstatus.txt","'$timestamp'")'
root -l -b -q '~/script/SubmitJob/jobstatus/jobstatus.C("Jobstatus/'$datafile'","'$timestamp'")'

#root -l -b -q jobstatus.C
#
# display Jobstatus/jobstatus_${timestamp}.gif


#percent=`cat log/8DD6E01DA337BFE3AFBDE9D25353DBCA_995.out | grep -ri "process" | sed -n '$p' | awk '{print $2}'`
#number=${percent%\%*}
#echo $number
