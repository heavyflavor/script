#!/bin/bash 
. ~/bin/FunPool.sh
date 

for irun in $@
do 
    hsi<<EOF
find /home/starsink/raw/daq/2015/$irun/ -name "*.daq" -print 
# ls /home/starsink/raw/daq/2015/$1/*/*.daq 
q
EOF
done 

