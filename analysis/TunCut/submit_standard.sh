#!/bin/bash
. ~/bin/ColorText.sh

date
count=0

if [ ! -d script ];then 
    mkdir script
else
    rm -f script/*
fi 
if [ ! -d log ];then
    mkdir log
else
    rm -f log/*
fi
mydir=`pwd`
LOG_INFO "=================================="
LOG_INFO "STAR Submit Jobs......"
LOG_INFO "Current dir is ${mydir}"

dca12=('0.006' '0.007' '0.008')
dca3=('0.007' '0.008' '0.009')
dca123=('0.005' '0.006' '0.007')
pt12=('0.9' '1.0' '1.1' '1.2')
pt3=('1.0' '1.1' '1.2' '1.3')
costheta=('0.98' '0.99' '0.995')
beta=('0.04' '0.05')

for idca12 in ${dca12[@]}
do 
    for idca3 in ${dca3[@]}
    do 
	for idca123 in ${dca123[@]}
	do 
	    for ipt12 in ${pt12[@]}
	    do 
		for ipt3 in ${pt3[@]}
		do 
		    for icostheta in ${costheta[@]}
		    do 
			for ibeta in ${beta[@]}
			do 
			    #if [ $count -gt 20 ]
			    #then 
			    #	exit 1
			    #fi 
			    cp standard.con run_${count}.con

			    echo "Initialdir     = ${mydir}" >> run_${count}.con 
			    echo "Executable     = standard.sh" >> run_${count}.con
			    echo "Arguments      = $idca12 $idca3 $idca123 $ipt12 $ipt3 $icostheta $ibeta $count" >> run_${count}.con 
			    echo "Output         = log/${count}.out" >> run_${count}.con
			    echo "Error          = log/${count}.err" >> run_${count}.con
			    echo "Log            = log/${count}.log" >> run_${count}.con
			    echo "Queue"         >> run_${count}.con
    
			    condor_submit run_${count}.con 
			    LOG_INFO "Submitting ${count}th jobs..."
			    mv run_${count}.con script/
			    let "count+=1"	    
    
			done 
		    done 

		done 

	    done 

	done 
	
    done 

done 

LOG_INFO "==============Summary============ "
LOG_INFO "Submited ${ifile} Jobs. "
