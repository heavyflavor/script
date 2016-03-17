#!/bin/bash 

mystr="sst_pedestals_16166032_s1_r2.root"

echo "----Get str length---"
echo ${#mystr}
expr length $mystr

echo "----find the position.---"
expr index $mystr "a"

echo "----find the position.---"
echo ${mystr:16:3}
expr substr "$mystr" 17 3 