#!/usr/bin/perl

# Read wavelength-based scintillation spectrum and normalize it
$n = 0;
while(<>) {
    chomp;
    if ($_ =~ m/^([0-9\.]+)\s+([0-9\.]+)\r$/) {
	$wl[$n] = $1;
	$tr[$n] = $2;
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

printf "\n\@tr = (";
for($i=0;$i<$n;$i++){
    if ($i%8 == 0) { printf "\n      "; }
    printf "%7.4f",$tr[$i];
    if ($i != $n-1) { printf ", "; }
}
printf " );\n";
