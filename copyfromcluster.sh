#!/bin/bash

date

if [ $# -ne 3 ]
then
    echo -e "\033[31mPlease input three parameters . byebye\033[0m"
    exit 1
fi

clustername=$1
clusterdir=$2
localdir=$3

if [ "$clustername" = "rcf" ]
then
    rsync -avrz zhoulong@rftpexp.rhic.bnl.gov:$clusterdir $localdir
elif [ $clustername = "pdsf" ]
then 
    rsync -avrz zhoulong@pdsf.nersc.gov:$clusterdir $localdir
else
    echo -e "\033[31mWrong cluster name , please check. byebye\033[0m"
    exit 1
fi

