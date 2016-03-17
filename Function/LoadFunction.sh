#!/bin/bash

. ~/bin/ColorText.sh

date 

LOG_INFO "Test New "

ls ./PWGCF*

if [ $? -ne 0 ]
then 
    LOG_ERROR "No Such Files , please check it! "
    exit 1
fi 

for ifile in `ls PWGCF*` 
do 
    LOG_INFO $ifile
    filename=`echo $ifile | awk -F '_' '{print $4}'`
    LOG_INFO $filename
    mv $ifile $filename
done 
