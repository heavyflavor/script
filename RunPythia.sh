!/bin/bash

date 


if [ $# -ne 1 ]
then
   echo -e "\033[31mPlease input one argument ! \033[0m"
cat <<EOF
Discribe: This macro used to do pythia simulation work ."
Argument: flag
flag = 1 :run jobs.
flag = 0 :skip run jobs.
Here is an example how to use this script.
1>if you want to do all the steps:
./RunPythia.sh 0
2>if you only want to check the job status and results.
./RunPythia.sh 1
EOF
   exit 1 
fi 
echo -e "Step 1 : Create simulation data(Only Trigger D0)\033[0m"

if [ $1 -eq 1 ]
then 
  ./runAll.sh 0 100
else 
  echo -e "\033[32mStep 1 : Create Simulation data Done !\033[0m"
fi 

myworks=1

while [[ myworks -eq 1 ]] 
do 
  echo -e "\033[32mCheck jobs.....\033[0m"
  condor_q zhoulong | tee |  grep -ri "0 jobs" 
  if [ $? -eq 0 ]
  then 
     ((myworks=0))
     echo -e "\033[32mJobs finish! \033[0m"
  else 
     echo -e "\033[32mJobs not finish yet , sleep 100 second..\033[0m"
     sleep 100
     #for ((i=0;$i<100;i+=1)) 
     #do 
        #printf "progress:[%-100s]%d%%\r" $b $i
        #echo -n "$i"
        #sleep 1
        #b=#$b
     #done  
  fi 
   
done 

if [ -e D0Spectrum.root ]
then
   rm -f D0Spectrum.root 
fi 

hadd D0Spectrum.root out/* 

echo -e "\033[32mStep 2 : Calculate D0 Pt spectrum. \033[0m"

if [ -e PythiaD0.root ]
then 
   rm PythiaD0.root
fi   

root -l -b -q  'plots.C("D0Spectrum.root","PythiaD0.root")'

echo -e "\033[32mStep 3 : Look at the Data vs Pythia \033[0m"

cp PythiaD0.root PythiaD0500.root

if [ ! -e getppReference.C ]
then
   cp ../getppReference.C ./
fi 

if [ ! -e ppReference.root ]
then 
   cp ../ppReference.root ./
fi 

if [ ! -e PythiaD0200.root ]
then
   cp ../PythiaD0200.root ./
fi 

root -l -b -q getppReference.C

echo -e "\033[32mWork Done ! \033[0m"
