#!/bin/csh


echo "=============================================="
echo " Mission Start    at `date`"
echo "=============================================="

@ Ts = `date +%s`
foreach folder ( `find -type d` )

	echo "  $folder"
	touch $folder/*

end 
@ Te = `date +%s`

@ Hor = ( $Te - $Ts ) / 3600
@ Min = ( $Te - $Ts ) % 3600 / 60
@ Sec = ( $Te - $Ts ) % 3600 % 60

echo "=============================================="
echo " Mission Complete at `date`"
echo ""
echo " Time Used : \033[36m$Hor : $Min : $Sec\033[0m"
echo ""
echo "=============================================="
