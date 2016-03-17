#!/bin/bash 

date 

start=$(date +%s)

. ~/bin/ColorText.sh

LOG_INFO "Test The Timer . "

sleep 10

end=$(date +%s)

diff=$(( end - start ))
LOG_INFO "Time take to excute command is ${diff} seconds."
