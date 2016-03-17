#!/bin/bash

date

if [ ! -e script ]
    then 
    mkdir script 
else 
    rm -f script/*
fi 


energy=('1.0' '1.2' '1.4' '1.6' '1.8' '2.0' '3.5' '4.0' '4.5' '5.0' '5.5' '6.0' '6.5' '7.0' '7.5' '8.0' '8.5' '9.0' '9.5')

for ifile in ${energy[@]}
  do
  echo "Input Energy is : $ifile"

  if [ ! -e ${ifile}/log ]
  then 
      mkdir -p ${ifile}/log
  else 
      rm -f ${ifile}/log/*
  fi 

  cp simulation.mac job${ifile}.mac
  echo "/gun/particle e-          " >> job${ifile}.mac
  echo "/gun/energy ${energy} GeV " >> job${ifile}.mac
  echo "/run/printProgress 10000  " >> job${ifile}.mac
  echo "/run/beamOn 200000        " >> job${ifile}.mac
  #echo "/run/beamOn 100           " >> job${ifile}.mac

  echo "Universe     = vanilla                                 " >> job${ifile}.con                                
  echo "GetEnv       = true                                    " >> job${ifile}.con
  echo "Initialdir   = /data2/xlchen/bso_simulation/3/${ifile} " >> job${ifile}.con
  echo "Executable   = bso                                     " >> job${ifile}.con
  echo "Arguments    = job${ifile}.mac                " >> job${ifile}.con 
  echo "GetEnv       = True                                    " >> job${ifile}.con
  echo "OnExitRemove = TRUE                                    " >> job${ifile}.con
  echo "Log          = log/run_${ifile}.log                    " >> job${ifile}.con
  echo "Output       = log/run_${ifile}.out                    " >> job${ifile}.con
  echo "Error        = log/run_${ifile}.error                  " >> job${ifile}.con
  echo "Queue                                                  " >> job${ifile}.con


  mv job${ifile}.mac $ifile     
  mv job${ifile}.con $ifile     

  cd ${ifile}
  if [ ! -e bso ]
  then 
      cp ../bso ./
  fi 
  condor_submit job${ifile}.con   
  cd ..
done

