#!/bin/bash
. ~/bin/FunPool.sh

count=0

if [ $# -eq 0 ]
then 
    nlines=10
else 
    nlines=$1
fi 

#firstline=0
#lastline=$(($firstline+$nlines))

scriptdir="script"
listdir="list"
stdout="log"
stderr="log"
outdir="out"

if [ ! -e "$scriptdir" ]
then 
    mkdir $scriptdir 
else 
    rm -f $scriptdir/
fi 

if [ ! -e "$listdir" ]
then 
    mkdir $listdir 
else 
    rm -f $listdir/
fi 

if [ ! -e "$stdout" ]
then 
    mkdir $stdout 
else 
    rm -f $stdout/
fi 

if [ ! -e "$stderr" ]
then 
    mkdir $stderr/ 
else 
    rm -f $stderr
fi 

split -l $nlines pico.list list_

mydir=`pwd`
echo "Current dir is ${mydir}"

for mylist in `ls list_*`
do
    #count=`echo $inlist | awk -F '_' '{print $2}'`
    inlist="list_${count}.list"
    mv $mylist ${listdir}/${inlist}

    outname="${count}.v0tree.root"

    cp standard.con run_${count}.con

    echo "Initialdir     = ${mydir}" >> run_${count}.con 
    echo "Executable     = standard.sh" >> run_${count}.con
    echo "Arguments      = ${listdir}/${inlist} ${outdir}/${outname}" >> run_${count}.con 
    echo "Output         = ${stderr}/${count}.out" >> run_${count}.con
    echo "Error          = ${stderr}/${count}.err" >> run_${count}.con
    echo "Log            = ${stderr}/${count}.log" >> run_${count}.con
    echo "Queue"         >> run_${count}.con
                           
    condor_submit run_${count}.con 

    # echo "starver pro" >> run_$count.csh
    # echo -n "run.sh ">>run_$count.csh
    # echo -n ${listdir}/${inlist} ${outdir}/${outname} >>run_$count.csh
    #     
    # qsub -hard -l h_vmem=0.4G,scratchfree=500,eliza14io=1,eliza17io=1 -o stdout/job_$count.log -e stderr/job_$count.err run_$count.csh 
    # mv run_$count.csh ${scriptdir}/

    mv run_$count.con ${scriptdir}/

    let "count=count+1" 
done   

LOG_INFO "++++++++++++++++++++++++++++"
LOG_INFO "+          Summary         +"
LOG_INFO "++++++++++++++++++++++++++++"
LOG_INFO "Input List : ${listdir}/"
LOG_INFO "OutPut Dir : ${outdir}/"
LOG_INFO "Log and err: ${stderr}/"
LOG_INFO "All $count Jobs are submitted !"
