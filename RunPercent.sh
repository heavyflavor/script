#!/bin/bash 
. ~/bin/FunPool.sh

date 

file_all=$1
file_part=$2
count=0
foundcount=0
num_all=`wc -l $file_all | awk '{print $1}'`
# percent=$(echo "scale=1;${count} / ${num_all} * 100" | bc) # "scale" used the set display precision. 
# echo "$percent%"
# exit 0 

# ((residue=${count}%1000))
# echo "$residue"
# if [ $residue -eq 0 ]
# then 
#     percent=$(echo "scale=1;${count} / ${num_all} * 100" | bc) # "scale" used the set display precision. 
#     LOG_INFO "PROCESSING $percent%..."
# fi 
# exit 0

if [ -e new.list ]
then 
    rm new.list
fi 

touch new.list
((oneper=$num_all/100))

for ifile in `cat $file_all`
do 
    ((residue=${count}%$oneper))
    if [ $residue -eq 0 ]
    then 
	percent=$(echo "scale=1;${count} / ${num_all} * 100" | bc) # "scale" used the set display precision. 
	LOG_INFO "PROCESSING $percent%..."
    fi 
    filename=${ifile##*\/}

    # findflag=`cat $file_part | awk '/'$filename'/{print}'` 
    grep "$filename" $file_part > /dev/null 

    if [ $? -eq 0 ]
    then 
	let "count+=1"
	# LOG_INFO "Not New file,Skip it. "
	continue
    else 
	let "foundcount+=1"
	# LOG_INFO "Find New file. "
	echo $ifile >> new.list
    fi 
    # if [ "${findflag}aa" = "aa" ]
    # then 
    # 	LOG_ERROR "Missing data : $filename"
    # 	continue
    # fi 

    # LOG_INFO "Input File is : $ifile"
    # LOG_INFO "File number   : $count "
    # LOG_INFO "File name is  : $filename"
    # LOG_INFO "-----------------------------------------------------"
    let "count+=1"
done 

LOG_INFO " ------------------------------- "
LOG_INFO "| Found New Files : $foundcount |"
LOG_INFO "|         Work Done!            |"
LOG_INFO " ------------------------------- "
