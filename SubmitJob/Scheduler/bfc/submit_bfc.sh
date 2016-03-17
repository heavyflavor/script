#!/bin/bash
date

if [ $# -ne 1 ]
then 
    echo -e "\033[31mWrong Input Arguments ! Try agian ! \033[0m" 
    exit 1
fi 

run=`basename $1 .lis`

echo -e "\033[32mInput Run number is : $run \033[0m" 

if [ ! -d output/${run} ]; then
    mkdir output/${run}
fi

if [ ! -d log/${run} ]; then
    mkdir log/${run}
fi

cd output/${run}

if [ -e LocalLibraries.zip ]
then 
    rm -f LocalLibraries.zip
fi 

cp ../../submit.xml .

star-submit-template -template submit.xml -entities runnumber=${run}

cd ../..


