Testing HDF5 on the Ed's NAM dataset, chromosome 2 (10M markers)

The C programs are compiled with h5cc(), the HDF5 C compiler. 
The data are loaded with loadNAM.c and queried with the other C
programs.  The NAM dataset is stored in a single HDF5 "dataset"
within the HDF5 file gobii.h5.  Only the marker variant calls are
stored, not the names of the samples or markers.  The storage format is
a two-dimensional array of char (1 byte), with samples in dimension 1
("horizontal") and markers in dimension 0 ("vertical").  That is, each
sample is a column and each marker is a row.  In practical use, the
names of the samples and markers and their indexes in the HDF5 array,
as well as the HDF5 filename and dataset name, would be stored as 
metadata in a relational database.

loadNAM.c
This program takes five arguments: The input csv file, the number of
samples, the number of markers, the name of the HDF5 file and the name of
the HDF5 dataset in that file.  The input file was
"/shared_data/NAM_HM32/csv/c2.csv", 9.4M markers by 5K samples, or this
file plus c1.csv. Load time was 24 minutes for c2 only, 55 minutes for
both.

Test Cases

1. All markers for N samples
samplestest.c takes three arguments: the number of samples, whether they
are distributed randomly or as a contiguous block, and the total number
of markers in the HDF5 dataset.  The results are stored in file
"samplestest.out", and the elapsed time in "samplestest.log".

2. All samples for N markers
markerstest.c takes three arguments: the number of markers, whether they
are distributed randomly or as a contiguous block, and the total number
of samples in the HDF5 dataset.  The results are stored in file
"markerstest.out", and the elapsed time in "markerstest.log".

3. N markers for M samples
   A. Contiguous block of markers and samples
haploblock.c takes two arguments: the number markers and the number
of samples.  The results are stored in file "haplotest.out" and the
elapsed.time in "haplotest.log".
   B. Random distribution of markers and samples
h5fetchbatch.c takes as arguments up to 60 pairs of 
<Sample number> <Marker number> [<Sample number> <Marker number>] ...
It is called repeatedly by the Perl script haplofetch.pl, which stores
all the results in a two-dimensional N x M array in memory, then writes
the array to the csv file "haplotest.csv" and logs the elapsed time in
"haplotest.log".  Times were recorded separately for the first step of
reading into the memory array and the second step of writing out to the
disk csv file.  The second step was less than 1% of the total.

Note:
For the bigger test cases it's necessary to increase the system stacksize
to avoid running out of memory.  In csh, "limit stacksize unlimited".
