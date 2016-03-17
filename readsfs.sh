#!/bin/bash
date

   echo "Input file name is: $1"
   infile=`basename $1 .sfs`
   echo "Input file prefix name: $infile"
if [ -f ${infile}.txt ];then
   rm -f ${infile}.txt
fi
./StRoot/RTS/src/RTS_EXAMPLE/rts_example -Dsst $1 > ${infile}.txt
#  ls -lrht 
#../Convert "${infile}.txt" "${infile}.bin"

echo "Work Done!!"
