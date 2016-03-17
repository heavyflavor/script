#!/bin/bash 

date 

cat <<EOF
++++++++++++++++++++++++++++++++++++++++
Test Function in Bash shell. 
This function can create random password. 
The argument is the length of password.
++++++++++++++++++++++++++++++++++++++++
EOF

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
