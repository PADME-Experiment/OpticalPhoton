#!/usr/bin/perl

$nevents = 100;
@e = ( 10, 50, 100, 200, 500, 1000 ); # Gamma energy in MeV
@l = ( 10, 12, 14, 16, 18, 20 ); # Crystal length in cm
@a = ( 1, 2, 5, 10 ); # Absorption length scale

printf "Scan for %d points\n",@e*@l*@a;

foreach $e (@e) {
    foreach $l (@l) {
	foreach $a (@a) {

	    $f = sprintf("pbf2_%dMeV_%dcm_%dfabs",$e,$l,$a);
	    $log = "prod/$f.log";
	    $mac = "prod/$f.mac";

	    $p = 0.5*$l+0.1;

	    open($fh,'>',$mac) or die "Could not write file $mac";
	    print $fh "/control/verbose 2\n";
	    print $fh "/tracking/verbose 0\n";
	    print $fh "/OpNovice/gun/mode 1\n";
	    print $fh "/gun/particle gamma\n";
	    print $fh "/gun/energy $e MeV\n";
	    print $fh "/gun/position 0. 0. -$p cm\n";
	    print $fh "/gun/direction 0. 0. 1.\n";
	    print $fh "/OpNovice/detector/mode 1\n";
	    print $fh "/OpNovice/detector/pbf2_crystal_length $l cm\n";
	    print $fh "/OpNovice/detector/pbf2_crystal_abslen_scale $a\n";
	    print $fh "/run/initialize\n";
	    print $fh "/OpNovice/phys/verbose 0\n";
	    print $fh "/run/beamOn $nevents\n";
	    close $fh;

	    $cmd = "time ./OpNovice -m $mac 1>$log 2>&1";
	    printf "$cmd\n";
	    system($cmd);

	}
    }
}
