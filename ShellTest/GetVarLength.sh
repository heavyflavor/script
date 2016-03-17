#!/bin/bash 
. ~/bin/ColorText.sh

if [ $# -eq 0 ]
then 
    temp="ABCDEFGH"
else 
    temp=$1
fi 

LOG_INFO "Input var is        : $temp"
LOG_INFO "Input Var Length is : ${#temp}"
