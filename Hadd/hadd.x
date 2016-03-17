#!/bin/csh

set files = ` awk '{print $1}' < file.list ` 
set i = 1
set m = 1
while ( $i <= $#files ) 
#while ( $i <= 3 )
  set k = ` expr $i - 1 `
  echo $k
 
	echo $files[$i]
    if ( $i < 2 ) then 
	 hadd $i.root $files[$i]
	else 
   	hadd $i.root $files[$i] $k.root
	rm -f $k.root
	endif
@ i++
end

