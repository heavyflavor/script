#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Please input unix time" 
    exit 1 
fi 

unixtime=$1

date -d @$unixtime +%Y%m%d.%H%M%S
