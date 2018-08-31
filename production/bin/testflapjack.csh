#!/bin/csh

# Filepaths to be adjusted appropriately:
set bindir = /home/dem3/hdf5/production/bin/
set workdir = /share/dem_hdf5_test/

if ($#argv < 1) then
    echo $0" <HDF5 file>"
    echo "Given: The markers-fast matrix in the HDF5 file is correct."
    echo "Test whether the flapjack (samples-fast) matrix is correct."
    echo "Examples: "
    echo $0" "$workdir"DS_27.h5.orig"
    echo $0" "$workdir"DS.reloaded.h5"
    exit
endif

echo "Testing file "$1

# Find the datumsize.
set datumsize = `$bindir"HDF5info" $1 | grep 'Datasize' | cut -d " " -f 2`

set infilename = `basename $1`

# Dump the markers-fast (correct) matrix.
$bindir"dumpdataset" markers-fast $1 $workdir"matrix.correct"

# Dump the samples-fast (flapjack) matrix.
$bindir"dumpdataset" samples-fast $1 $workdir"matrix.flapjack"

# Load the samples-fast matrix, i.e. the transposed version.
$bindir"loadHDF5" $datumsize $workdir"matrix.flapjack" $workdir$infilename".flipped"
# If it fails to load it must be incorrect.
if ($? != 0) then
    echo "The flapjack matrix is incorrect."
    exit
endif

# Dump the samples-fast version of the flipped h5 file, should equal the original markers-fast (matrix.correct).
$bindir"dumpdataset" samples-fast $workdir$infilename".flipped" $workdir"matrix.flapjack.flipped"
# Sometimes an incorrect matrix causes loadHDF5 to segfault, producing a file that dumpdataset can't read.
if ($? != 0) then
    echo "The flapjack matrix is incorrect."
    exit
endif

# If dumpdataset succeeded, compare the result with the original matrix.correct.
cmp $workdir"matrix.correct" $workdir"matrix.flapjack.flipped"
if ($? == 0) then
    echo "matrix.correct is identical to the flapjack matrix flipped."
endif

# Clean up the working files.
rm $workdir"matrix.correct"
rm $workdir"matrix.flapjack"
rm $workdir$infilename".flipped"
rm $workdir"matrix.flapjack.flipped"
