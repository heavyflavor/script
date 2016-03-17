#!/bin/bash

date
ifile=0
if [ ! -d myscript ];then 
   mkdir myscript
fi

if [ ! -d mylog ];then
   mkdir mylog
fi

if [ $# -ne 1 ];then
       #37m --white,30m--black
       echo -e "\033[31m ERROR: You must supply one argument. byebye \033[0m"
       #echo -e "\033[32m ERROR: You must supply one argument. byebye \033[0m"
       #echo -e "\033[41;37m ERROR: You must supply one argument. byebye \033[0m"
       #echo -e "\033[42;37m ERROR: You must supply one argument. byebye \033[0m"
       #echo -e "\033[44;37m ERROR: You must supply one argument. byebye \033[0m"
       #echo -e "\033[45;37m ERROR: You must supply one argument. byebye \033[0m"
       #echo -e "\033[46;37m ERROR: You must supply one argument. byebye \033[0m"
     
       exit 1
fi 

outdir=$1
if [ ! -d "$outdir" ];then
   mkdir "$outdir"
fi
 
rm -f mylog/*
rm -f myscript/*
mydir=`pwd`
echo "Current dir is ${mydir}"

  for FILE in `cat pedlist`
  do
    echo "Input data is : ${FILE}"
    logfile=`basename $FILE .Ped.root`
    echo $logfile
    
    cp standard.con run_${logfile}.con
    echo "Initialdir     = ${mydir}" >> run_${logfile}.con
    echo "Executable     = RunMySelector.sh" >> run_${logfile}.con
    echo "Arguments      = ${FILE} ${outdir}" >> run_${logfile}.con 
    echo "Output         = mylog/${logfile}.out" >> run_${logfile}.con
    echo "Error          = mylog/${logfile}.err" >> run_${logfile}.con
    echo "Log            = mylog/${logfile}.log" >> run_${logfile}.con
    echo "Queue"         >> run_${logfile}.con
                           
    condor_submit run_${logfile}.con 
                                              
    mv run_${logfile}.con myscript/
    let "ifile+=1"
done


