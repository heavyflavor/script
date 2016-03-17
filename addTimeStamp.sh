#!/bin/bash 
. ~/bin/FunPool.sh

date 

for ifile in `ls tables/`
do 
    prefix=`echo $ifile | awk -F '.' '{print $1}'`
    suffix=`echo $ifile | awk -F '.' '{print $3}'`
    runid=`echo $ifile | awk -F '.' '{print $2}'`

    unixtime=`cat r14production.list | awk '/'$runid'/{print $2}'` 

    if [ "${unixtime}aa" = "aa" ]
    then 
	LOG_ERROR "Run $runid is not a good Run."
	continue
    fi 

    itimestamp=`date -d @$unixtime +%Y%m%d.%H%M%S`
    #convertUnixTimeStamp.sh $unixtime

    outname="${prefix}.${itimestamp}.${suffix}"

    cp "tables/${ifile}" "StarDb/$outname"

    LOG_INFO "Input File is : $ifile"
    LOG_INFO "Unix time  is : $unixtime"
    LOG_INFO "OutName  is   : $outname"
    LOG_INFO "-----------------------------------------------------"

done 

LOG_INFO " ----------------------------- "
LOG_INFO "|         Work Done!          |"
LOG_INFO " ----------------------------- "
