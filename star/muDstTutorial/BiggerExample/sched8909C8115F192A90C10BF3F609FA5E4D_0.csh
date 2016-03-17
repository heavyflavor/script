#!/bin/csh
# -------------------------------------- 
# Script generated  at Thu May 31 04:50:49 EDT 2012 by the STAR scheduler and submitted with
# cd /star/u/jlzhang/muDstTutorial/BiggerExample; condor_submit /star/u/jlzhang/muDstTutorial/BiggerExample/sched8909C8115F192A90C10BF3F609FA5E4D_0.condor
# --------------------------------------

onintr  eviction



/bin/echo 'We are starting on node: '`/bin/hostname`


# Preparing environment variables

setenv FILEBASENAME "no_name"
setenv FILELIST "/star/u/jlzhang/muDstTutorial/BiggerExample/sched8909C8115F192A90C10BF3F609FA5E4D_0.list"
setenv FILELIST_ALL "/star/u/jlzhang/muDstTutorial/BiggerExample/sched8909C8115F192A90C10BF3F609FA5E4D.list"
setenv INPUTFILECOUNT "2"
setenv JOBID "8909C8115F192A90C10BF3F609FA5E4D_0"
setenv JOBINDEX "0"
setenv LOGGING "STD"
setenv REQUESTID "8909C8115F192A90C10BF3F609FA5E4D"
setenv SUBMITATTEMPT "1"
setenv SUBMITTINGDIRECTORY "/star/u/jlzhang/muDstTutorial/BiggerExample"
setenv SUBMITTINGNODE "rcas6015.rcf.bnl.gov"
setenv SUBMIT_TIME "2012-05-31 08:50:49"
setenv SUMS_AUTHENTICATED_USER "jlzhang@rhic.bnl.gov"
setenv SUMS_USER "jlzhang"
setenv SUMS_nProcesses "1"
setenv SUMS_name "null"
setenv USEXROOTD "1"
setenv INPUTFILE0 "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/productionCentral/FullField/P05ic/2004/033/st_physics_5033131_raw_1010002.MuDst.root"
setenv INPUTFILE1 "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/productionCentral/FullField/P05ic/2004/033/st_physics_5033131_raw_1010001.MuDst.root"

# This big blobk is used by CONDOR where $HOME in not set

if ( ! $?USER ) then
    echo "USER is not defined"
    set USER=`id | sed "s/).*//" | sed "s/.*(//"`
endif
if ( ! $?HOME ) then
    echo "HOME is not defined"

    if ( -x /usr/bin/getent ) then
	# we have getent, should not be on aix, bsd, Tru64 however
	# will work for Linux
	echo "Using getent method"
	setenv HOME `/usr/bin/getent passwd $USER | /bin/sed 's|.*\:.*\:.*\:.*\:\([^\:]*\):.*|\1|'`
    else 
	set PTEST=`which perl`
	if ( "$PTEST" != "" ) then
	    echo "Using perl method"
	    # we have perl defined, we can get info from there
	    /bin/cat <<EOF >test$$.pl
my(\$user) = getpwuid(\$<);
@items = getpwnam(\$user);
print \$items[7];
EOF
	    setenv HOME `$PTEST test$$.pl` && /bin/rm  -f test$$.pl
	else
	    set CTEST=`which cc`
	    if ( "$CTEST" != "" ) then
		echo "Using C code method"
		# use C code for doing this
		/bin/cat <<EOF >test$$.c
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

int main()
{
  struct passwd *info;
  /* assume \$USER is defined */
  const char *user=getenv("USER");
  info = getpwnam(user);
  (void) printf("%s",info->pw_dir);

}

EOF
		$CTEST -o test$$ test$$.c
		/bin/chmod +x test$$
		setenv HOME `./test$$` && /bin/rm -f test$$ test$$.c
	    else
		echo "We have no ways to define HOME and it is not defined"
		exit
	    endif
	endif
    endif
endif

echo "HOME is now $HOME"


# Default value for path if not defined.
if ( ! $?PATH ) then
   setenv PATH /usr/local/bin:/bin:/usr/bin
endif


/usr/bin/test -e $HOME/.cshrc && source $HOME/.cshrc

# Creating the scratch directory, return failure status
set SUMS_try=0
set SUMS_PAD=""

MKDTRY:
setenv SCRATCH "/tmp/$USER$SUMS_PAD/$JOBID"
/bin/mkdir -p $SCRATCH   >& /dev/null
set STS=$status
if (! -d $SCRATCH) then
       #test if porper UID
       set SUMS_IsUID=`(/usr/bin/test -O /tmp/$USER$SUMS_PAD && echo 1) || echo 0`
       if ( $SUMS_try == 0 &&  -e "/tmp/$USER$SUMS_PAD" && ! $SUMS_IsUID) then
            # First try, directory exists but is not owned by $USER
            # Create a different path and try again
            echo "Scheduler:: $SCRATCH not owned by $USER, trying alternative"
            @ try++
            set SUMS_seed=`/bin/date +%j%H%M%S`
            set SUMS_PAD=`expr $SUMS_seed  \* 25973 \% 100`
            goto MKDTRY
        else
		 echo "Scheduler:: Failed to create $SCRATCH on $HOST"
	         exit $STS
        endif

endif


#import packages 
/usr/bin/unzip sched8909C8115F192A90C10BF3F609FA5E4D_0.package.zip -d $SCRATCH


#Note: The default directory in which jobs start has been fix to $SCRATCH
cd $SCRATCH

echo "--------------------------------"
echo 'STAR Unified Meta Scheduler 1.10.7 we are starting in $SCRATCH :' `/bin/pwd`
echo "--------------------------------"
/bin/ls -l

###################################################
# User command BEGIN ----------------------------->
 
        starpro
        cons   
        root4star -q -b BiggerAnalysis.C\(0,$INPUTFILECOUNT,\"$FILELIST\",\"$SCRATCH\",\"$JOBID\"\) 

# <------------------------------User command BEGIN
###################################################

# Copy output files (if any where specified)
/bin/cp -r $SCRATCH/*.root /star/u/jlzhang/muDstTutorial/BiggerExample/

# Delete the scratch directory
/bin/rm -fr $SCRATCH




# Function to pass batch system signals to the process, if need
eviction:
set PIDS=`ps  -o "%p" --no-headers --ppid $$`
foreach  process ($PIDS)
  set killit=`ps -o "%p" --no-headers -p $process`
  if ( "x$killit" !~ "x" ) then
     echo "evicting `ps  --no-headers -p$process`"
     kill -15 $process
  endif
end
wait
