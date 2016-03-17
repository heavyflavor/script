#!/bin/bash 

date 

cat <<EOF
This micro is used to create Time dependency logfile, this is useful, because when you want to re-oritation the output to a logfile, usually if you are not delete the old one, you may met errors , but if the logfile is time dependency, you will never met those errors. 
EOF

timestamp=`date +%Y%m%d%H%M%S`

ls * > ${timestamp}.log
