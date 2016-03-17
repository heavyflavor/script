#!/bin/bash
 
date 

echo `pwd`

echo -e "\033[32m===============================\033[0m"
echo -e "\033[32mStart do some clearence work ..\033[0m"
echo -e "\033[32mClear log files..\033[0m"
if [ ! -e log ]
then
   mkdir log 
else 
   rm -f log/*
fi

if [ ! -e Log ]
then 
  mkdir Log
else 
  rm -f Log/*
fi 
 
echo -e "\033[32mClear OutPut files..\033[0m"
if [ ! -e out ]
then
  mkdir out 
else 
  rm -f out/*
fi 
echo -e "\033[32mClear submit info..\033[0m"
if [ ! -d csh ]
then 
  mkdir csh 
else 
  rm -f csh/*
fi 

if [ ! -d list ]
then 
  mkdir list 
else 
  rm -f list/*
fi 
 
if [ ! -d report ]
then 
  mkdir report 
else 
  rm -f report/*
fi

echo -e "\033[32mClearning old .xml files... \033[0m"
rm -f *.session.xml
echo -e "\033[32mClearning old dataset files... \033[0m"
rm -f *.dataset
rm -f *.dataset.tmp
echo -e "\033[32mClearning old package and zip... \033[0m"
rm -f LocalLibraries.zip
rm -rf LocalLibraries.package/
 
echo -e "\033[32mClearence work Done ! \033[0m"
echo -e "\033[32m===============================\033[0m"
echo -e "\033[32mStart Submit jobs ....\033[0m"
star-submit submit.xml

#sed -i 's/PicoDst/\/star\/institutions\/lbl_prod\/Run13_pp510GeV\/MB_HT\/PicoProd/g' 076.list
