#!/bin/bash

date

dirname=`echo $1 | awk -F '_' '{print $3;}'`

echo "dir name is : $dirname"

mv $1 174/$dirname

#ls Run14/ | grep "st_" | xargs -L 1 move.sh
