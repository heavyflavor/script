#!/bin/bash

timestamp=`date`
echo -e "\033[32m $timestamp \033[0m"

if [ $# -ne 3 ];then
   echo -e "\033[31m ERROR: You must supply three argument. byebye \033[0m"
   echo -e "\033[31m INFO:Input Arguments is 1>source code name, 2>version number , 3>discribe"
   echo -e "\033[31m Example 1: ./UpdateCode.sh mycode.cxx 1.2 \"This is my first version \""
   echo -e "\033[31m Example 2: upcode mycode.cxx 1.3 \"\\\n//\<1\>Just test my back up code.\\\n//\<2\>check what happen.\\\n//\<3\>Just an example.\""
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

if [ $suffix = "sh" -o $suffix = "csh" ];then
   echo -e "\033[32m Input file is a shell script. \033[0m"
   echo -e "\033[32m Start Updating ${sourcecode} ... \033[0m"
   sed -i -e '$a\\' $sourcecode #add an void line.
   sed -i -e '$a\#===============================' $sourcecode
   sed -i -e '$a\#Version: '"${version}"' ' $sourcecode
   sed -i -e '$a\#Time: '"${timestamp}"' ' $sourcecode
   sed -i -e '$a\#Author: Long Zhou ' $sourcecode
   sed -i -e '$a\#Discribe: '"${discribe}"' ' $sourcecode
   echo -e "\033[32m Update Done ! \033[0m"
   exit 1 
elif [ $suffix = "cxx" -o $suffix = "C" -o $suffix = "cpp" ];then
   echo -e "\033[32m Input file is a C or C++ code. \033[0m"
   echo -e "\033[32m Start Updating ${bkcode} ... \033[0m"

   headerfile="${prefix}.h"

   if [ -f $headerfile ];then
      echo -e "\033[32m Header File: $headerfile \033[0m"
      sed -i -e '1i\\' $headerfile
      sed -i -e '1i\//Discribe: '"${discribe}"' ' $headerfile
      sed -i -e '1i\//Author: Long Zhou ' $headerfile
      sed -i -e '1i\//Time: '"${timestamp}"' ' $headerfile
      sed -i -e '1i\//Version: '"${version}"' ' $headerfile
      sed -i -e '1i\//===============================' $headerfile
      echo -e "\033[32m Header File Done ! \033[0m"
   fi
   sed -i -e '1i\\' $sourcecode
   sed -i -e '1i\//Discribe: '"${discribe}"' ' $sourcecode
   sed -i -e '1i\//Author: Long Zhou ' $sourcecode
   sed -i -e '1i\//Time: '"${timestamp}"' ' $sourcecode
   sed -i -e '1i\//Version: '"${version}"' ' $sourcecode
   sed -i -e '1i\//===============================' $sourcecode
   echo -e "\033[32m Update Done ! \033[0m"
fi

