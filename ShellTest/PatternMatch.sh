#!/bin/bash 

date 

infile=`basename $1 .root`

timelable=${infile##*_}

echo "$timelable"
