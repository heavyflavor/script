#!/bin/bash 

date 

args=('Test.root' '2' '2' '3' 'PDF')
count=0
for i in $@
do 
    args[$count]=$i
    let "count+=1"
done 

if [ $# -lt 1 ]
then 
    echo -e "\033[31m Wrong input, must have at lest one arguments! \033[0m"
    exit 1
fi 

infile=`basename ${args[0]} .root `
mydir=`dirname ${args[0]}`
nrow=${args[1]}
ncol=${args[2]}
optword=${args[3]}        # optword is the coding with logy and logz. formate : 0xlogylogz  eg. 1 - logz, 2- logy, 3-logy and logz
outdir=${args[4]}
if [ ! -e $outdir ]
then 
    mkdir $outdir
fi 

echo -e "\033[32m Input File is  : ${mydir}/${infile}.root \033[0m"
echo -e "\033[32m OutPut File is : ${outdir}/${infile}.pdf \033[0m"
echo -e "\033[32m Out Canvas is  : $nrow x $ncol \033[0m"
root -l -b -q '~/script/Plots/makeQaPlotsOnPdf.C("'${mydir}'","'${outdir}'","'${infile}'",'${nrow}','${ncol}','${optword}')'

echo -e "\033[32mDone! \033[0m"
