#!/bin/bash

date 

echo -e "\033[32mThis macros used to test while statement.\033[0m"

myworks=1

while [[ myworks -eq 1 ]] 
do 
  condor_q zhoulong | grep -ri "0 jobs" 
  if [ $? -eq 0 ]
  then 
     ((myworks=0))
  else 
     sleep 500  
  fi 
   
done 



echo -e "\033[32mAll jobs finished ! \033[0m"
 
