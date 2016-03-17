#!/bin/bash
. ~/bin/FunPool.sh

date

LOG_INFO "Step one : Compile Code "
make 

if [ $? -ne 0 ]
then 
    LOG_ERROR "Compile Failed, please check code first."
    exit 1 
fi 

ifile=0

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
echo "Current dir is ${mydir}"

  for FILE in `cat run.list`
  do
    echo "Input data is ${FILE}"
    fileid=`basename $FILE .v0tree.root | awk -F '_' '{print $2}'`
    echo "File ID is : $fileid"

    cp standard.con run_${fileid}.con

    echo "Initialdir     = ${mydir}" >> run_${fileid}.con 
    echo "Executable     = standard.sh" >> run_${fileid}.con
    echo "Arguments      = $FILE" >> run_${fileid}.con 
    echo "Output         = log/${fileid}.out" >> run_${fileid}.con
    echo "Error          = log/${fileid}.err" >> run_${fileid}.con
    echo "Log            = log/${fileid}.log" >> run_${fileid}.con
    echo "Queue"         >> run_${fileid}.con
                           
    condor_submit run_${fileid}.con 
                                              
    mv run_${fileid}.con script/
    let "ifile+=1"
done


