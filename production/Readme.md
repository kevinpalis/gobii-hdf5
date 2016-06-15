# Using loadHDF5

To use `bin/loadHDF5`, you must first set the system stacksize to unlimited.
In csh, "limit stacksize unlimited".

The application takes two arguments: the name of the input Intermediate
Format file, and the name of the destination HDF5 file.  The data are
stored in two HDF5 *datasets* in the file, named "allelematrix"
(original orientation) and "allelematrix_samples-fast" (transposed).

`bin/loadPhased` is similar except that it handles 2-character SNP calls
like "GG" as well as 1-character IUPAC codes.  So it takes another 
argument (first), the datatype, either "Phased" or "IUPAC".

The HDF5 file can be browsed with `h5dump()`.

# Dumping a dataset

`bin/dumpdataset` outputs the specified HDF5 file to the specified
output file as a tab-delimited line for each sample.  The first argument
is the orientation desired for the output, "samples-fast" or
"markers-fast".

# Using the "fetch" programs

`bin/fetchsample` takes three arguments: the name of the source HDF5
file, the index position of the desired sample, and the name of the
output file.  Output is a one-line file with the alleles of all markers
for that sample, tab-delimited.

`bin/fetchmarker` takes three arguments: the name of the source HDF5
file, the index position of the desired marker, and the name of the
output file.  Output is a one-line file with the alleles of all samples
for that marker, tab-delimited.

`bin/fetchpoints` takes at least four arguments: the name of the source
HDF5 file, the name of the output file, and at least one pair of (index
position of sample, index position of marker), all punctuated only by
blanks.  Up to sixty sample/marker pairs can be requested, which is
faster than running the program sixty times.

All programs output a Usage message and a working example if they are
invoked with no arguments.

# Compiling the C programs

Use "h5cc -O".  (That's an oh, not a zero.)
