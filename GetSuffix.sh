#!/bin/bash
 
date 

echo `pwd`

infile=$1
suffix=${infile##*.}

echo -e "\033[32mInput file name is : ${infile}\033[0m"
echo -e "\033[32mInput file type is : ${suffix}\033[0m"
