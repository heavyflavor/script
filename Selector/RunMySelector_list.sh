#!/bin/bash

cd `pwd`

starver DEV
if test $# -lt 3
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

if [ ! -d $2 ];then
   mkdir $2
fi

file=$1
echo "input file is $file"

root -l -b -q 'RunMySelector_list.C("'$file'","'$2'","'$3'")'

