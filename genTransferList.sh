#!/bin/bash

date
DAY=$1
RUN=$2

if [ ! -d ${DAY}/${RUN} ]; then
  mkdir ${DAY}/${RUN}
fi

for FILE in `cat ${DAY}/${RUN}.list`
do
      echo $FILE
      echo -n "/home/starsink/raw/daq/2014/">>daq_${RUN}.lis
      echo -n ${DAY}>>daq_${RUN}.lis
      echo -n "/">>daq_${RUN}.lis
      echo -n ${RUN}>>daq_${RUN}.lis
      echo -n "/">>daq_${RUN}.lis
      echo -n $FILE>>daq_${RUN}.lis
      echo -n " /star/institutions/lbl_prod/hft/Run14/daq/">>daq_${RUN}.lis
      echo -n "${DAY}">>daq_${RUN}.lis
      echo -n "/">>daq_${RUN}.lis      
      echo -n ${RUN}>>daq_${RUN}.lis
      echo -n "/">>daq_${RUN}.lis
      echo $FILE>>daq_${RUN}.lis
      
done
      mv daq_${RUN}.lis ${DAY}
