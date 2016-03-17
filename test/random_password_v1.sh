#!/bin/bash 

date 

. /star/u/zhoulong/bin/ColorText.sh
cat <<EOF
++++++++++++++++++++++++++++++++++++++++
Test Function in Bash shell. 
This function can create random password. 
The argument is the length of password.
++++++++++++++++++++++++++++++++++++++++
EOF

ColorText 1 10 4 "Create PassWord : Length is $1 "

function rpass() {  
        cat /dev/urandom | tr -cd '[:graph:]' | head -c ${1:-12}  
	echo ""
}  

if [ $# -eq 0 ]
then 
    rpass 6
else 
    rpass $1
fi 
