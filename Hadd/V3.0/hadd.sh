#!/bin/bash

date 

echo -e "\033[32m++++++++++++++++++++++++++++++++++++++++++++++\033[0m"
echo -e "\033[32mVersion : 3.0\033[0m"
echo -e "\033[32mAuthor  : Long Zhou\033[0m"
echo -e "\033[32mTime    : 2014/5/22\033[0m"
echo -e "\033[32mDescribe: This macro is an standard program used to add histograms.\033[0m"
echo -e "\033[32m++++++++++++++++++++++++++++++++++++++++++++++\033[0m"

echo -e "\033[32mBefore run this code, need do some clearance . \033[0m"
if [ -d haddinfo ]
then 
   rm -rf haddinfo  
fi

if [ -e myhadd.xml ]
then
   rm -f myhadd.xml
fi 
echo -e "\033[32mClearance Done ! \033[0m"

mydir=`pwd`
myInputNumber=$#

echo -e "\033[32mInput arguments number is $myInputNumber \033[0m"

case $myInputNumber in 
  0)
   echo -e "\033[32mCorrent Mode is TEST Mode . Please Check the output files. \033[0m"
   root4star -l -b -q 'myhadd.cpp("'$mydir'")';;  #test mode 
  1)
   echo -e "\033[32mStart creating xml file and submit jobs ... \033[0m"
   root4star -l -b -q 'myhadd.cpp("'$mydir'",'$1')';;
  2)
   echo -e "\033[32mStart creating xml file and submit jobs ... \033[0m"
   if [ ! -e "$2" ]
   then 
     echo -e "\033[31mERROR: \" $2 \" No such file or directory, please check it ! \033[0m"
     exit 1
   fi 
   root4star -l -b -q 'myhadd.cpp("'$mydir'",'$1',"'$2'")';;
  3)
   echo -e "\033[32mStart creating xml file and submit jobs ... \033[0m"

   if [ ! -e "$2" ]
   then 
     echo -e "\033[31mERROR: \" $2 \" No such file or directory, please check it ! \033[0m"
     exit 1
   fi 

   if [ ! -e "$3" ]
   then 
     echo -e "\033[31mERROR: \" $3 \" No such file or directory, please check it ! \033[0m"
     exit 1
   fi 
   root4star -l -b -q 'myhadd.cpp("'$mydir'",'$1',"'$2'","'$3'")';;
  4)
   echo -e "\033[32mStart creating xml file and submit jobs ... \033[0m"

   if [ ! -e "$2" ]
   then 
     echo -e "\033[31mERROR: \" $2 \" No such file or directory, please check it ! \033[0m"
     exit 1
   fi 

   if [ ! -e "$3" ]
   then 
     echo -e "\033[31mERROR: \" $3 \" No such file or directory, please check it ! \033[0m"
     exit 1
   fi 
   root4star -l -b -q 'myhadd.cpp("'$mydir'",'$1',"'$2'","'$3'",'$4')';;
  *)
   echo -e "\033[31mWrong Input Argumtns. Please look at the below instructions,and Try again ! \033[0m"
   cat<<EOF
=========================================================
How to Use this macro:
Example:
   ./hadd.sh runmode "SourceDir" "DestinationDir" NfilesPerJob
   SourceDir: Your input root file dir name(absolute path);
   DestinationDir: Your output file dir name(absolute path);
   NfilesPerJob: How many files will accumate and submit a job;
   TEST Mode: runmode = 1;
   HADD Mode: runmode = 0;

note:Submit infor and StdOut,StdError are in $DestinationDir/haddinfo/
=========================================================
EOF
   exit 1;;
esac

echo -e "\033[32mWork Done! \033[0m "
