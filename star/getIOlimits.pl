#!/usr/bin/env perl
#
#
#  simple script to query the SGE complex-resources
#  & query their usage
#
#############################################


%global_resource;
%global_left;

getGlobalResources();
getGlobalUsage();

for my $key (keys %global_resource) {
    my $total = $global_resource{$key};
    my $left  = $global_left{$key};
    my $used = $total - $left;
    my $fused = 100.;
    if($total>0.){
     $fused =100.0*$used/$total;
    }
    $readaval= sprintf("%.1f",$fused);
    $remain= sprintf("%.0f",$left);
    print "SGE GlobalResource: Total $key = $total   \t Available=$remain    \t   Used= $readaval \%\n";
}  


#-------------------------------------


sub getGlobalResources
{

    my $glbcmd = qq{ qconf -se global };
    my @glbs = `$glbcmd`;

    for($i=0;$i<=$#glbs;$i++){

	if($glbs[$i]=~m/\=/){
	    $glbs[$i]=~ s/complex_values//;
	    $glbs[$i] =~ s/\s+//g;
	    getIOlist($glbs[$i]);

	}
    }

}


#-------------------------------------

sub getGlobalUsage 
{


    my $globalQuery=qq{qhost -F };

    for my $key (keys %global_resource) {
	my $val = $global_resource{$key};
	$globalQuery=$globalQuery.$key.",";
    }

    chop $globalQuery;
    $globalQuery=$globalQuery." -h pdsf";

    my @ress=`$globalQuery`;

    for($i=0;$i<=$#ress;$i++){
	if($ress[$i]=~m/gc\:/){
	    $ress[$i]=~ s/\s+//g;
	    getIOStatus($ress[$i]);
	}
    }

}

#-------------------------------------

sub getIOStatus 
{
    local(@s1);
    @s1=split("gc\:",$_[0]);
    @s2=split("\=",$s1[1]);
    $global_left{$s2[0]}=$s2[1];

}

#-------------------------------------

sub getIOlist
{

    local(@s1);
    @s1=split("\,",$_[0]);
    for($j=0;$j<=$#s1;$j++){
        if($s1[$j]=~m/\=/){
	    my @s2=split("\=",$s1[$j]);
            $global_resource{$s2[0]}=$s2[1];
	}
    }
}
