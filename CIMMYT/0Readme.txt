Testing HDF5 on the CIMMYT SeeD Beagle-imputed dataset from DataVerse

The C programs are compiled with h5cc(), the HDF5 C compiler. 
The data are loaded with storeseed.c and queried with the other C
programs.  The CIMMYT dataset is stored in a single HDF5 "dataset"
within the HDF5 file gobii.h5.  Only the marker variant calls are
stored, not the names of the samples or markers.  The storage format is
a two-dimensional array of char (1 byte), with samples in dimension 1
("horizontal") and markers in dimension 0 ("vertical").  That is, each
sample is a column and each marker is a row.  In practical use, the
names of the samples and markers and their indexes in the HDF5 array,
as well as the HDF5 filename and dataset name, would be stored as 
metadata in a relational database.

storeseed.c
This program takes no arguments, they are hard-coded.  The input file
is "AllZeaGBSv2.7_SEED_Beagle4_ALL.hmp.txt" in the current directory.
This file is the catenation of the ten single-chromosome Hapmap csv
files from Dataverse, in chromosome order.  It outputs to file 
"gobii.h5", dataset "/maize".  Load time was ca. four minutes.

Test Cases

1. All markers for N samples
samplestest-cbsu.c takes two arguments: the number of samples, and
whether they are distributed randomly or as a contiguous block.  The
results are stored in file "samplestest.out", and the elapsed time
in "samplestest.log".

2. All samples for N markers
markerstest-cbsu.c takes two arguments: the number of markers, and
whether they are distributed randomly or as a contiguous block.  The
results are stored in file "markerstest.out", and the elapsed time in
"markerstest.log".

3. N markers for M samples
h5fetchbatch.c takes as arguments up to 60 pairs of 
<Sample number> <Marker number> [<Sample number> <Marker number>] ...
It is called repeatedly by the Perl script haplofetch.pl, which stores
all the results in a two-dimensional N x M array in memory, then
writes the array to the csv file "haplotest.csv" and logs the elapsed
time in haplotest.log.

Note:
For the bigger test cases it's necessary to increase the system stacksize
to avoid running out of memory.  In bash, "ulimit stacksize unlimited".
