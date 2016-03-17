#!/bin/bash

timestamp=`date`
echo $timestamp

if [ $# -ne 3 ];then
   echo -e "\033[31m ERROR: You must supply three argument. byebye \033[0m"
   echo -e "\033[31m INFO:Input Arguments is 1>source code name, 2>version number , 3>discribe"
   echo -e "\033[31m Example 1: ./BackCode.sh mycode.cxx 1.2 \"This is my first version \""
   echo -e "\033[31m Example 2: bkcode mycode.cxx 1.3 \"\\\n//\<1\>Just test my back up code.\\\n//\<2\>check what happen.\\\n//\<3\>Just an example.\""
   exit 1
fi 

sourcecode=$1

echo "Input File: $sourcecode"

prefix=`echo $sourcecode | awk -F '.' '{print $1}'`
suffix=`echo $sourcecode | awk -F '.' '{print $2}'`

version=$2
discribe=$3
echo "Version: $version"
echo "Discribe: $discribe"

bkcode="${sourcecode}-v${version}-"
bkheader="${header}-v${version}-"

if [ ! -d BK ];then 
   mkdir BK
fi 

cp $sourcecode $bkcode

header="${prefix}.h"

if [ -f $header ];then
   echo "Header File: $header"
   cp $header BK/$bkheader
fi
sed -i -e '1i\\n' $bkcode
sed -i -e '1i\//Discribe: '"${discribe}"' ' $bkcode
sed -i -e '1i\//Time: '"${timestamp}"' ' $bkcode
sed -i -e '1i\//Version: '"${version}"' ' $bkcode
sed -i -e '1i\//===============================' $bkcode

mv $bkcode BK/

