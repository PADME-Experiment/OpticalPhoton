#!/usr/bin/perl

# Read wavelength-based radiant sensitivity and convert to quantum efficiency
$n = 0;
while(<>) {
    chomp;
    if ($_ =~ m/^([0-9\.]+)\s+([0-9\.]+)$/) {
	$e[$n] = 1239.86/$1;
	$q[$n] = 1.23986*$2/$1;
	$n++;
    }
    if ($_ =~ m/^([0-9\.]+),\s+([0-9\.]+)$/) {
	$e[$n] = 1239.86/$1;
	$q[$n] = 1.23986*$2/$1;
	$n++;
    }
}

printf "\n\@ep = (";
for($i=0;$i<$n;$i++){
    if ($i%10 == 0) { printf "\n      "; }
    printf "%8.6f",$e[$n-$i-1];
    if ($i != $n-1) { printf ", "; }
}
printf " );\n";

printf "\n\@qe = (";
for($i=0;$i<$n;$i++){
    if ($i%10 == 0) { printf "\n      "; }
    printf "%8.6f",$q[$n-$i-1];
    if ($i != $n-1) { printf ", "; }
}
printf " );\n";
