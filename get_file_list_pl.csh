#!/bin/bash

date 

echo `pwd`

cat <<EOF
=============================
Name    :get_file_list_pl.csh
Autor   :Long Zhou
Time    :Aug 3,2014
Describe:
  This script used to get file list,
  and create an adequate filelist.
  (used to submit jobs !)
=============================
EOF
if [ $# -eq 0 ];then 
   outlist=Run13pp500.list
elif [ $# -eq 1 ];then
   outlist=$1
else 
   echo -e "\033[31mInput argument is not correct,please check it !\033[0m"
   exit 1
fi 

echo -e "\033[32mOutput Filelist is $outlist\033[0m"

#get_file_list.pl -keys "path,filename" -cond "storage=hpss,production=P11id,trgsetupname=AuAu19_production,filetype=daq_reco_MuDst,filename~st_physics,tof=1" -o Run11_19GeV_P11id.list -delim '/' -limit 0

#get_file_list.pl -keys "path,filename" -cond "storage=hpss,production=P14ia,trgsetupname=pp500_production_2013,filetype=daq_reco_MuDst,filename~st_physics,tof=1" -o Run13pp500.list -delim '/' -limit 5000

#get_file_list.pl -keys "path,filename" -cond "storage=hpss,production=P14ia,trgsetupname=pp500_production_2013,filetype=daq_reco_MuDst,filename~st_physics" -o Run13pp500.list -delim '/' -limit 5000

get_file_list.pl -keys "node,path,filename" -cond "storage=hpss,production=P14ia,trgsetupname=pp500_production_2013,filetype=daq_reco_MuDst,filename~st_physics" -o $outlist -delim '/' -limit 0

#get_file_list.pl -keys "path,filename" -cond "storage=hpss,production=P10ik,trgsetupname=AuAu62_production,filetype=daq_reco_MuDst,filename~st_physics,tof=1" -o Run10_62GeV_P10ik.list -delim '/' -limit 0

#get_file_list.pl -keys "path,filename" -cond "storage=hpss,production=P10ik,trgsetupname=AuAu200_production,filetype=daq_reco_MuDst,filename~st_physics,tof=1" -o Run10_200GeV_P10ik.list -delim '/' -limit 0

sed -i 's/localhost\/\//\/\/localhost\//g' $outlist
nfiles=`wc -l ${outlist}`
echo -e "\033[32m******SUMMARY******\033[0m"
echo -e "\033[32mOutput Filename:${outlist}\033[0m"
echo -e "\033[32mFile     Number:${nfiles}\033[0m"
echo -e "\033[32mWork Done!\033[0m"
echo -e "\033[32m*******************\033[0m"
