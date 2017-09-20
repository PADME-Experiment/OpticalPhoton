#!/usr/bin/perl

# Read wavelength-based scintillation spectrum and normalize it
$n = 0;
while(<>) {
    chomp;
    if ($_ =~ m/^([0-9\.]+), ([0-9\.]+)$/) {
	$wl[$n] = $1;
	$vwl[$n] = $2;
	$n++;
    }
}

printf "\n\@wl = (";
for($i=0;$i<$n;$i++){
    if ($i%8 == 0) { printf "\n      "; }
    printf "%8.4f",$wl[$i];
    if ($i != $n-1) { printf ", "; }
}
printf " );\n";

printf "\n\@vwl = (";
for($i=0;$i<$n;$i++){
    if ($i%8 == 0) { printf "\n      "; }
    printf "%6.4f",$vwl[$i];
    if ($i != $n-1) { printf ", "; }
}
printf " );\n";
