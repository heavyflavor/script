#!/bin/bash 

date 

if [ -e jobstatus.txt ]
then
    rm jobstatus.txt
fi 

touch jobstatus.txt

if [ ! -e Jobstatus ]
then 
    mkdir Jobstatus
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
    echo "$number" >> jobstatus.txt
done 

timestamp=`date +%Y%m%d%H%M%S`

root -l -b -q 'jobstatus.C("jobstatus.txt","'$timestamp'")'

#root -l -b -q jobstatus.C
#
display Jobstatus/jobstatus_${timestamp}.gif


#percent=`cat log/8DD6E01DA337BFE3AFBDE9D25353DBCA_995.out | grep -ri "process" | sed -n '$p' | awk '{print $2}'`
#number=${percent%\%*}
#echo $number
