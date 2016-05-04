# Using loadHDF5

To use `bin/loadHDF5`, you must first set the system stacksize to unlimited.
In csh, "limit stacksize unlimited".

The application takes two arguments: the name of the input Intermediate
Format file, and the name of the destination HDF5 file.  The data are
stored in two HDF5 *datasets* in the file, named "allelematrix"
(original orientation) and "allelematrix_samples-fast" (transposed).

The HDF5 file can be viewed and browsed with `h5dump()`.
