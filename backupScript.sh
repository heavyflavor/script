#!/bin/bash
backUp(){

local sourceDir=$1     #need backup dir
local destDir=$2         #destination dir

if [ -d "$sourceDir" ] #backup dir 
then   
    #if source dir is not backup , create this dir 
    if [ ! -d "$destDir/$sourceDir" ]
    then
        echo -e "Creating\t$sourceDir"
        mkdir  -p "$destDir/$sourceDir"
    fi
    #loop all files in this dir,recursive call this backup function to do backup work .
    for var in $(ls "$sourceDir")
    do               
        backUp "$sourceDir/$var" "$destDir"
    done
else     #back up file
    #if this file was not backed up, then copy this file directly  
    if [ ! -f "$destDir/$sourceDir" ]
    then
        echo -e "Backing up file\t$sourceDir"
        cp "$sourceDir" "$destDir/$sourceDir"
    else
        #if file already exists,compare last modify time,if file had been changed,then update file
        lastModifi_backup=$(date -r "$destDir/$sourceDir" +%s)
        lastModifi_current=$(date -r "$sourceDir" +%s)
        if [ "$lastModifi_current" -gt "$lastModifi_backup" ]
        then
            echo -e "Updating file\t$sourceDir"           
            cp "$sourceDir" "$destDir/$sourceDir"           
        fi
    fi
fi
}
backUp "$1" "$2"
