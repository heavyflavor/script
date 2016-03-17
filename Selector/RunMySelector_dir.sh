#!/bin/bash
. ~/bin/FunPool.sh

cd `pwd`

if test $# -gt 2
   then
       #37m --white,30m--black
       echo -e "\033[31m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[32m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[41;37m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[42;37m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[44;37m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[45;37m ERROR: You must supply two argument. byebye \033[0m"
       echo -e "\033[46;37m ERROR: You must supply two argument. byebye \033[0m"
     
       exit 1
fi 

if [ $# -eq 2 ]
then 
    number=$2
else 
    number=5e9
fi 

file=$1
LOG_INFO "Input Dir is : $file"

root -l -b -q 'RunMySelector_dir.C("'$file'",'$number')'

LOG_INFO "Done! "
