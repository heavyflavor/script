#!/bin/bash 

date 
args=('15137059' '0')

count=0
for i in $@
do 
    args[$count]=$i
    let "count+=1"
done 

echo -e "\033[32mInput Run number is : $input \033[0m"

root -l -b -q 'ana_bso.C("'${args[0]}'",'${args[1]}')'

echo -e "\033[32mCreate QA Plots.\033[0m"

./makeQaPlotsOnPdf.sh run_${args[0]}_hist.root

echo -e "\033[32mCopy pdf file to ~/Documents\033[0m"

cp run_${args[0]}_hist.pdf ~/Documents/
mv run_${args[0]}_hist.pdf out/
mv run_${args[0]}_hist.root out/

echo -e "\033[32mWork Done! \033[0m"
