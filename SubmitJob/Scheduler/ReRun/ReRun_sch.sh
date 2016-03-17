#!/bin/bash
. ~/bin/FunPool.sh

date
 
function showhelp()
{
    cat<<EOF
=========================================================
How to Use this macro:
Example:
   ./ReRun_sch.sh runmode "All_jobs_list" "Finished_jobs_list" "submit_xml" "submit_file_list"
   All_job_list: All jobs list, usually you need create it by yourself. ls list/* > All_jobs_list;
   Finished_jobs_list: Finished jobs list, you need create it by youself. ls out/* > Finished_jobs_list
   Submit_xml: your submit jobs xml file name
   Submit_file_list:The file list in your submit jobs xml file.
   runmode : 0 - create rerun xml file and list, you can check the xml file and then submit by yourself
             1 - create rerun xml file and list, then submit it automatically
=========================================================
EOF
}

echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"
echo -e "\033[32m+ Name    : ReRun.sh                                                  +\033[0m"
echo -e "\033[32m+ Version : 1.0                                                       +\033[0m"
echo -e "\033[32m+ Author  : Long Zhou                                                 +\033[0m"
echo -e "\033[32m+ Time    : 2014/10/21                                                +\033[0m" 
echo -e "\033[32m+ Describe: This macro only used to Re-Run faild jobs[not idel jobs]  +\033[0m"
echo -e "\033[32m+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\033[0m"

echo `pwd`

if [ ! -e ReRun ]
then 
    mkdir ReRun
fi 

# Defaults arguments
iarg=0
args=('0' 'joblist.list' 'outlist.list' 'submit_local.xml' 'pico.list') # submitjobs_flag All_job_list finished_job_list submit_xml all_submit_filelist
for i in $@
do 
    args[$iarg]=$i
    let "iarg+=1"
done 

if [ "$1" = "-h" -o "$1" = "-help" -o "$1" = "--help" -o "$1" = "--h" ]
then 
    showhelp
    exit 1 
fi 

# Initialize parameters
issubmit=${args[0]}
file_all=${args[1]}
file_part=${args[2]}
submitxml=${args[3]}
submitlist=${args[4]}
# outtreedir=${args[5]}

# Set output and rerun xml and initialize lists
rerunxml="re_$submitxml"
timestamp=`date +%Y%m%d%H%M%S`
outname="${timestamp}.list"
finishedlist="${timestamp}_finished.list"

touch $outname
touch $finishedlist
count=0
foundcount=0
finishcount=0
num_all=`wc -l $file_all | awk '{print $1}'`
((oneper=$num_all/100))
echo "All runs: $num_all"
echo "One percent run : $oneper"

# Find the missing lists 
for ifile in `cat $file_all`
do 
    if [ $count -lt 10 ]
    then
	echo "$ifile"
    fi 

    ((residue=${count}%$oneper))
    if [ $residue -eq 0 ]
    then 
	percent=$(echo "scale=2;${count} / ${num_all} * 100" | bc) # "scale" used the set display precision. 
	LOG_INFO "PROCESSING $percent%..."
    fi 

    filename=`basename ${ifile##*sched} .list`	# Can be replaced by command  "basename".
    grep "$filename" $file_part > /dev/null 

    if [ $? -eq 0 ]
    then 
	let "finishcount+=1"
	((count+=1))
        # LOG_INFO "Not New file,Skip it. "
	cat $ifile >> $finishedlist	    
	# continue
    else 
	let "foundcount+=1"
	((count+=1))
	LOG_ERROR "Find ${foundcount}th bad run. "
	cat $ifile >> $outname
    fi 

    # let "count+=1"
done 

# # Prepare for submit jobs
mkdir -p ReRun
mv $outname ReRun/
# ln -sf ReRun/$outname rerun.list

if [ $issubmit -eq 1 ]
then 
    if [ -e $rerunxml ]
    then
	rm $rerunxml
    fi
    cp $submitxml $rerunxml
    # sed -i 's/pico.list/rerun.list/g' $rerunxml
    # sed -i 's/pico.list/ReRun\/'"$outname"'/g' $rerunxml 
    sed -i 's/'"$submitlist"'/ReRun\/'"$outname"'/g' $rerunxml 
    star-submit $rerunxml
fi 

LOG_INFO "-------------------------------------------"
LOG_INFO " Job list is         : $file_all "
LOG_INFO " Out list is         : $file_part "
LOG_INFO " All File number     : $num_all "
LOG_INFO " Fished File number  : $finishcount "
LOG_INFO " Missing File Number : $foundcount "
LOG_INFO " Output List is      : $outname"
LOG_INFO "-------------------------------------------"

# Remind you when this work done !
# echo -e "\07";
# sleep 1;
# echo -e "\07";
# sleep 1;
# echo -e "\07";
# sleep 1;
# echo -e "\07";
# sleep 1;
# echo $'\07';

echo -e "\033[32mWork Done !\033[0m"

#===============================
#Version: 1.0 
#Time: Sat Oct 25 17:37:28 EDT 2014 
#Author: Long Zhou 
#Discribe: This macro used to Re-submit idle jobs 

#===============================
#Version: 1.1 
#Time: Sun Oct 26 22:06:31 EDT 2014 
#Author: Long Zhou 
#Discribe: This version code can re-submit all jobs or only idel jobs 

#===============================
#Version: 1.2 
#Time: Thu Dec  3 10:20:55 PST 2015 
#Author: Long Zhou 
#Discribe: directly use the difference between output file list and job list to identify the bad jobs, and resubmit 

#===============================
#Version: 1.3 
#Time: Thu Dec  3 10:41:41 PST 2015 
#Author: Long Zhou 
#Discribe: Fixed few small bugs  

#===============================
#Version: 1.4 
#Time: Thu Dec  3 11:38:22 PST 2015 
#Author: Long Zhou 
#Discribe: Added some help information 
