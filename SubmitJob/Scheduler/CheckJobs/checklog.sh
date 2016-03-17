#!/bin/bash 

date 

echo -e "\033[32m Check Jobs logs \033[0m"

cd `pwd`

for jobid in `condor_q -wide zhoulong | awk '{print $9}' | xargs -I aaa basename aaa .csh | awk -F '_' '{print $2}'`
do 
    cat log/*${jobid}.*
done 
