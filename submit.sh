#!/bin/bash
date

run=$1
max=$2

if [ ! -d output/${run} ]; then
  mkdir output/${run}
fi


    count=0
    while [ ${count} -lt ${max} ]
    do

      echo ${count}
      

      if [ ! -d output/${run}/${count} ]; then
        mkdir output/${run}/${count}
      fi
    
      cd output/${run}/${count}

      start=${count}
      end=`expr ${count} + 199`
    
      echo ${start} ${end}

      cp ../../../submit.xml .
      
      star-submit-template -template submit.xml -entities runnumber=${run},start=${start},stop=${end}

      let "count+=200"
      
      cd ../../..

    done

