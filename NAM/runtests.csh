#!/bin/csh

# Run the HDF5 timing tests.

# Case 1, all markers for a specified number of samples.
samplestest 100 contiguous 9434936
samplestest 1000 contiguous 9434936
samplestest 3000 contiguous 9434936

samplestest 100 random 9434936
samplestest 1000 random 9434936
samplestest 3000 random 9434936

# Case 2, all samples for a specified number of markers.
# markerstest 1000 contiguous 5258
# markerstest 3000 contiguous 5258
# markerstest 10000 contiguous 5258
# markerstest 30000 contiguous 5258
# markerstest 100000 contiguous 5258

# markerstest 1000 random 5258
# markerstest 3000 random 5258
# markerstest 10000 random 5258
# markerstest 30000 random 5258
# markerstest 100000 random 5258

# Case 3, M markers for N samples
# Contiguous
#haploblock 1000 1000
# haploblock 3000 3000
# haploblock 30000 3000

# Random
#haplotest.pl <number of markers> <number of samples>
#haplotest.pl 1000 1000
# haplotest.pl 3000 3000
# haplotest.pl 10000 1000
# haplotest.pl 30000 3000
