#!/usr/bin/perl -w

# haplotest.pl, 29apr2017, from: haplotest.pl, DEM 28jan2016
# from haplofetch.php, DEM 29dec2015, from randomfetch.php
# Test performance of looking up alleles for an arbitrary set of markers for a set of samples with h5fetchbatch().
# Results are in 2D array alleles[sample][marker].

use strict;
use warnings;

my ($i, $j, $m, $n, @markers, @samples, @answers, @alleles);
my ($answerbatch, $anssize, $ansnumber);
open (my $logfile, ">>", "haplotest.log");

if (scalar(@ARGV) < 2) {
    print "Usage: haplotest.pl <number of markers> <number of samples>\n";
    print "Fetch marker variant calls for a random set of markers and samples.\n";
    print "Output is in files /tmp/haplotest.out and ./haplotest.log\n";
    exit;
}

my $markercount = $ARGV[0];
my $samplecount = $ARGV[1];
my $datapoints = $markercount * $samplecount;

# The limit seems to be around 60.  70 segfaults.  Limit on size of argv[]?  Line length?
my $batchlimit = 60;

# Choose the markers.
for ($i = 0; $i < $markercount; $i++) {
    $markers[$i] = int(rand(31617212));
}
# Choose the samples.
for ($i = 0; $i < $samplecount; $i++) {
    $samples[$i] = int(rand(5258));
}

print ($logfile "Fetching allele calls for $markercount markers and $samplecount samples.\n");
my $starttime = time();
my $batch = "";
my $batchsize = 0;
$m = 0; $n = 0;
for ($i = 0; $i < $samplecount; $i++) {
    for ($j = 0; $j < $markercount; $j++) {
	# Build the string of arguments to the h5fetchbatch() command.
	# $batch .= "$markers[$j] $samples[$i] ";
	$batch .= "$samples[$i] $markers[$j] ";
	$batchsize++;
	if ($batchsize == $batchlimit) {
	    # Send the batch of requests to HDF5 and collect the results.
	    # $answerbatch = system ("h5fetchbatch $batch");
	    # $answerbatch = `bin/h5fetchbatch $batch`;
	    $answerbatch = `bin/fetchpoints /local/data/NAM_HM32/NAMc6-10.h5 /tmp/fetchpoints.out $batch`;
	    chop $answerbatch;
	    @answers = split(/ /, $answerbatch);
	    $anssize = scalar(@answers);
	    $ansnumber = 0;
	    while ($ansnumber < $anssize) {
		$alleles[$m][$n] = $answers[$ansnumber];
		$n++;
		if ($n == $markercount) {
		    $n = 0; 
		    $m++;
		}
		$ansnumber++;
	    }
	    $batch = "";
	    $batchsize = 0;
	}
    }
}
# Flush-through the last partial batch if any.
$answerbatch = `bin/fetchpoints /local/data/NAM_HM32/NAMc6-10.h5 /tmp/fetchpoints.out $batch`;
chop $answerbatch;
@answers = split(/\t/, $answerbatch);
$anssize = scalar(@answers);
$ansnumber = 0;
$m = 0; 
$n = 0;
while ($ansnumber < $anssize) {
    $alleles[$m][$n] = $answers[$ansnumber];
    $n++;
    if ($n == $markercount) {
	$n = 0; 
	$m++;
    }
    $ansnumber++;
}
my $elapsed = time() - $starttime;
print ($logfile "$datapoints datapoints fetched in $elapsed seconds.\n");
print ($logfile "Writing the output file...\n");

# Output the results.
open (my $outfile, ">", "/tmp/haplotest.out");
for ($i = 0; $i < $markercount; $i++) {
    for ($j = 0; $j < $samplecount; $j++) {
	print ($outfile $alleles[$j][$i]."\t");
    }
    print ($outfile "\n");
}
close($outfile);
$elapsed = time() - $starttime;
print ($logfile "Total time = $elapsed seconds.\n\n");
close($logfile);

