#!/bin/bash

timestamp=`date`
echo -e "\033[32m=====================================\033[0m"
echo -e "\033[32mSTART BACKUP SOURCECODE....\033[0m"
echo -e "\033[32m $timestamp \033[0m"

if [ $# -ne 3 ];then
   echo -e "\033[31m ERROR: You must supply three argument. byebye \033[0m"
   echo -e "\033[31m INFO:Input Arguments is 1>source code name, 2>version number , 3>discribe"
   echo -e "\033[31m Example 1: ./BackCode.sh mycode.cxx 1.2 \"This is my first version \""
   echo -e "\033[31m Example 2: ./BackCode.sh mycode.cxx 1.3 \"\\\n//\<1\>Just test my back up code.\\\n//\<2\>check what happen.\\\n//\<3\>Just an example.\""
   exit 1
fi 

sourcecode=$1

echo -e "\033[32m Input File: $sourcecode \033[0m"

prefix=`echo $sourcecode | awk -F '.' '{print $1}'`
suffix=`echo $sourcecode | awk -F '.' '{print $2}'`

version=$2
discribe=$3
echo -e "\033[32m Version: $version \033[0m"
echo -e "\033[32m Discribe: $discribe \033[0m"

#bkcode="${sourcecode}"
bkcode="tempcode"
bkheader="tempheader"

if [ ! -d BK ];then 
   mkdir BK
fi 

bkdir="${prefix}_${version}"

if [ ! -d BK/$bkdir ];then
   mkdir BK/$bkdir
fi

cp $sourcecode $bkcode

if [ $suffix = "sh" -o $suffix = "csh" ];then
   echo -e "\033[32m Input file is a shell script. \033[0m"
   echo -e "\033[32m Start Backing up ${sourcecode} ... \033[0m"
   sed -i -e '$a\\' $bkcode #add an void line.
   sed -i -e '$a\#===============================' $bkcode
   sed -i -e '$a\#Version: '"${version}"' ' $bkcode
   sed -i -e '$a\#Time: '"${timestamp}"' ' $bkcode
   sed -i -e '$a\#Author: Long Zhou ' $bkcode
   sed -i -e '$a\#Discribe: '"${discribe}"' ' $bkcode
   mv $bkcode BK/$bkdir/$sourcecode
   echo -e "\033[32m BackUp Done ! \033[0m"
   #exit 1 
elif [ $suffix = "cxx" -o $suffix = "C" -o $suffix = "cpp" ];then
   echo -e "\033[32m Input file is a C or C++ code. \033[0m"
   echo -e "\033[32m Start Backing Up ${bkcode} ... \033[0m"

   headerfile="${prefix}.h"
   
   if [ -f $headerfile ];then
      cp $headerfile $bkheader
      echo -e "\033[32m Header File: $headerfile \033[0m"
      sed -i -e '1i\\' $bkheader
      sed -i -e '1i\//Discribe: '"${discribe}"' ' $bkheader
      sed -i -e '1i\//Author: Long Zhou ' $bkheader
      sed -i -e '1i\//Time: '"${timestamp}"' ' $bkheader
      sed -i -e '1i\//Version: '"${version}"' ' $bkheader
      sed -i -e '1i\//===============================' $bkheader
      mv $bkheader BK/$bkdir/$headerfile
      echo -e "\033[32m Header File Done ! \033[0m"
   fi
   sed -i -e '1i\\' $bkcode
   sed -i -e '1i\//Discribe: '"${discribe}"' ' $bkcode
   sed -i -e '1i\//Author: Long Zhou ' $bkcode
   sed -i -e '1i\//Time: '"${timestamp}"' ' $bkcode
   sed -i -e '1i\//Version: '"${version}"' ' $bkcode
   sed -i -e '1i\//===============================' $bkcode
   mv $bkcode BK/$bkdir/$sourcecode
   echo -e "\033[32m BackUp Done ! \033[0m"
fi

echo -e "\033[32m=====================================\033[0m"
echo -e "\033[32mUPDATING SOURCECODE......\033[0m"
~/script/UpdateCode.sh "$sourcecode" "$version" "$discribe"
echo -e "\033[32mUPDATE DONE! \033[0m"
