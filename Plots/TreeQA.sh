#!/bin/bash
. ~/bin/FunPool.sh

date 
# if [ $# -ne 1 ]
# then
#     LOG_ERROR "Wrong Input arguments."
#     exit 1
# fi
args=('ds_data.root' 'mDs' '5e4')
count=0
for i in $@ 
do 
    args[$count]=$i
    let "count+=1"
done

infile=`basename ${args[0]} .root`
treename=${args[1]}

root -l -b -q 'TreeQA.C+("'$infile'","'$treename'")'

# display pic/${infile}.gif
