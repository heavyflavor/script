#!/bin/csh

#############################################################
#															#
# This macro will back up all files with specified exention	#
# name from folder $FSource to folder $FTarget.				#
#															#
# File path in $FSource and $FTarget is identical			#
#															#
#############################################################

#============================================================
set FTarget = "./BACKUP_CODE"
set FSource = "."
#============================================================
mkdir -p $FTarget

echo "==========================="
echo " Mission Start"
echo "==========================="
foreach folder ( `find $FSource -type d` )
#	echo "check fold : $folder"
	if ( $folder != $FTarget ) then
		foreach fileX ( `ls $folder` )
#============================================================
			if ( -f $folder/$fileX ) then 
			#	echo "check file1: $fileX"
				set file = `echo "${fileX}" | cut -d '*' -f 1`
			#	echo "check file2: $file"
				set Ex = `echo $file | awk -F\. '{ print $NF}'`
				if ( $Ex == "cpp" || $Ex == "CPP" || $Ex == "Cpp" || \
					 $Ex == "cxx" || $Ex == "CXX" || $Ex == "Cxx" || \
					 $Ex == "c"   || $Ex == "C"   || $Ex == "h"   || $Ex == "H" || \
					 $Ex == "csh" || $Ex == "CSH" || $Ex == "bash"|| $Ex == "BASH" || \
					 $Ex == "tcsh"|| $Ex == "TCSH" ) then
					echo " Find $Ex file $file"
					mkdir -p $FTarget/$folder
					cp $FSource/$folder/$file $FTarget/$folder/$file
					echo "$FSource/$folder/$file $FTarget/$folder/$file"
				endif
			endif
#============================================================
		end
	endif
end 
echo "==========================="
echo " Mission Complete"
echo "==========================="

     
