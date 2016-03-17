#!/bin/bash

date

if [ $# -ne 3 ]
then
    echo -e "\033[31mPlease input three parameters . byebye\033[0m"
    exit 1
fi

clustername=$1
clusterdir=$3
localdir=$2

if [ "$clustername" = "rcf" ]
then
    rsync -avrz  $localdir zhoulong@rftpexp.rhic.bnl.gov:$clusterdir
elif [ $clustername = "pdsf" ]
then
    rsync -avrz  $localdir zhoulong@pdsf.nersc.gov:$clusterdir
else
    echo -e "\033[31mWrong cluster name, plase check. byebye\033[0m"
    exit 1
fi

