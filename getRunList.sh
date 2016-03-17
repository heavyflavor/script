#!/bin/bash
. ~/bin/FunPool.sh

date 

if [ -e run.list ]
then 
    rm run.list
fi

if [ ! -e ListRecords ]
then 
    mkdir ListRecords
fi 

myargs=('v0tree' '*.root')
count=0
for i in $@
do 
    myargs[$count]=$i
    let "count+=1"
done 

disdir=${myargs[0]}
filename=${myargs[1]}
realpath=`readlink $disdir`
LOG_INFO "Path : $realpath"
LOG_INFO "We will only get file with name \"${filename}\""
itimestamp=`date +%m%d`
outname="${myargs[0]}_${itimestamp}.list"

find "${realpath}" -name "$filename" > $outname

nfiles=`wc -l $outname `

cp $outname run.list
mv $outname ListRecords
cp run.list test.list 
sed -i '10,$d' test.list

LOG_INFO "Created V0tree Data List : $outname ! "
LOG_INFO "Number of Files : $nfiles "
