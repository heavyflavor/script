#!/bin/bash

date 

for ifile in `ls ./Run14Production/`
do 
    runnumber=`basename $ifile .cor.root | awk -F '_' '{print $2}'`
    echo $runnumber
done   
