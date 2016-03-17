#!/bin/bash
date 

if [ ! -d Run14 ]
then
   ln -s /direct/star+data03/daq/2014
fi

if [ -e pedlist ]
then
   rm -f pedlist
fi

ls -lrht Run14/*/*/* | grep -i "zhoulong" | awk -F " " '{print $9;}' | grep -i "pedestal"  > pedlist

#This code just used to get pedestla run list from daq file .
