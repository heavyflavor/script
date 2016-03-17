#!/bin/bash

#output color text, need 4 arguments: Frontend Color, background color, Text Style, Text
function ColorText()
{
    if [ $# -ne 4  ]
    then
	echo "Usage:4 parameters are needed!"
	echo "      ForeColor,BackgroundClor Style and Text!"
	ShowHelp;
    else
	Fg="3""$1"
	Bg="4""$2"
	SetColor="\E[""$Fg;$Bg""m"
	Style="\033[""$3""m"
	Text="$4"
	EndStyle="\033[0m"
	echo -e "$SetColor""$Style""$Text""$EndStyle"
    fi;
}

function ShowHelp()
{
    echo "The frst parameter is a number ranged from 1 to 10,represents the foreground color."
    echo "The second parameter is  a number ranged from 1 to 10,represents the background color."
    ColorText 1 8 2 "1    red";
    ColorText 2 8 2 "2    green";
    ColorText 3 8 2 "3    yellow";
    ColorText 4 8 2 "4    blue";
    ColorText 5 8 2 "5    magenta";
    ColorText 6 8 2 "6    cyan";
    ColorText 7 8 2 "7    gray";
    ColorText 8 8 2 "8    white";
    ColorText 9 8 2 "9    white";
    ColorText 10 8 2 "10   black";
    echo "The third parameter is a number ranged from 1 to 9,represents the style of the characters."
    ColorText 10 8 1 "1    lighter,and bold";
    ColorText 10 8 4 "4    draw a line under the string.";
    ColorText 10 8 7 "7    swap the foreground color and the background color";
    ColorText 10 8 9 "9    draw a deleting line";
    ColorText 10 8 9 "others  normal style";
    echo "The fourth parameter is the content you wanna clolor,a string."
}

function LOG_ERROR()
{
    if [ $# -ne 1 ]
    then
	echo "Please Input Error Message! "
    else  
	ColorText 1 8 2 "ERROR: $1"
    fi 
}

function LOG_INFO()
{
    if [ $# -ne 1 ]
    then
	echo "Please Input Info Message! "
    else  
	ColorText 2 8 2 "INFO: $1"
    fi 
}

function EMPT_DIR()
{
    if [ $# -ne 1 ]
    then 
	echo "Please Input the Directory name!"
	exit 1
    fi

    emptyDir=$1
    for ifile in `ls ${emptyDir}/*`
    do 
	echo -e "\033[31mDeleting $ifile .....\033[0m"
	rm -f $ifile
    done
}
#Show the Input text to screen .
#ColorText $@
