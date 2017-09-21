#!/usr/bin/perl

# Read wavelength-based quantum efficiency and normalize it
$n = 0;
while(<>) {
    chomp;
    if ($_ =~ m/^([0-9\.]+), ([0-9\.]+)$/) {
	$e[$n] = 1239.86/$1;
	$q[$n] = $2/100.;
	$n++;
    }
}

printf "\nG4double ePhoton[] = {";
for($i=0;$i<$n;$i++){
    if ($i%8 == 0) { printf "\n      "; }
    printf "%6.4f*eV",$e[$n-$i-1];
    if ($i != $n-1) { printf ", "; }
}
printf " };\n";

printf "\nG4double quantumEff[] = {";
for($i=0;$i<$n;$i++){
    if ($i%8 == 0) { printf "\n      "; }
    printf "%6.4f",$q[$n-$i-1];
    if ($i != $n-1) { printf ", "; }
}
printf " };\n";
