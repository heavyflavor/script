#!/bin/bash

cd /star/u/zhoulong/bnl/pedestal

starver DEV
if [ $# -ne 2 ];then
       echo "ERROR: You must supply two argument. byebye"
       exit 1
fi 

if [ ! -d "$2" ];then
   mkdir $2
fi
root -l -b -q 'RunMySelector.C("'$1'","'$2'")'

