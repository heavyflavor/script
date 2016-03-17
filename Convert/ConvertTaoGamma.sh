#!/bin/bash 
. ~/bin/ColorText.sh

date 

if [ $# -ne 2 ]
then 
    LOG_ERROR "Please input two arguments "
    exit 1
fi 

root -l -b -q 'ConvertTaoGamma.C('$1','$2')'  
