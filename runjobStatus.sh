#!/bin/bash

if [ $# -eq 1 ];then
   user=$1
   root -l -b -q 'runjobStatus.cpp("'${user}'")'
else
   root -l -b -q 'runjobStatus.cpp("zhoulong")'
fi


