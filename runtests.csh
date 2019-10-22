#!/bin/csh

# Run the HDF5 timing tests.

# Case 1, all markers for a specified number of samples.
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 4000 50 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 4100 100 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 4200 500 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 3000 1000 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 1900 2000 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 800 3000 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 500 4000 /local/data/NAM_HM32/fetchsampleblock.out

# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 50 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 100 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 500 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 1000 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 2000 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 3000 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 4000 /local/data/NAM_HM32/randomsamples.out

# Case 2, all samples for a specified number of markers.
# bin/fetchmarkerblock /local/data/NAM_HM32/NAMc6-10.h5 7000017 1000 /local/data/NAM_HM32/fetchmarkerblock.out
# bin/fetchmarkerblock /local/data/NAM_HM32/NAMc6-10.h5 8000018 5000 /local/data/NAM_HM32/fetchmarkerblock.out
# bin/fetchmarkerblock /local/data/NAM_HM32/NAMc6-10.h5 9000019 10000 /local/data/NAM_HM32/fetchmarkerblock.out
# bin/fetchmarkerblock /local/data/NAM_HM32/NAMc6-10.h5 10000020 50000 /local/data/NAM_HM32/fetchmarkerblock.out
# bin/fetchmarkerblock /local/data/NAM_HM32/NAMc6-10.h5 11000021 100000 /local/data/NAM_HM32/fetchmarkerblock.out
# bin/fetchmarkerblock /local/data/NAM_HM32/NAMc6-10.h5 12000022 500000 /local/data/NAM_HM32/fetchmarkerblock.out
# bin/fetchmarkerblock /local/data/NAM_HM32/NAMc6-10.h5 13000023 1000000 /local/data/NAM_HM32/fetchmarkerblock.out
# bin/fetchmarkerblock /local/data/NAM_HM32/NAMc6-10.h5 14000024 5000000 /local/data/NAM_HM32/fetchmarkerblock.out

# Try sending the output file to a different physical disk from the input.
# Use a different random list of markers for each run.
# bin/fetchmarkerlist_rand /local/data/NAM_HM32/NAMc6-10.h5 1000 /tmp/fetchmarkerlist.out
# bin/fetchmarkerlist_rand /local/data/NAM_HM32/NAMc6-10.h5 5000 /tmp/fetchmarkerlist.out
# bin/fetchmarkerlist_rand /local/data/NAM_HM32/NAMc6-10.h5 10000 /tmp/fetchmarkerlist.out
# bin/fetchmarkerlist_rand /local/data/NAM_HM32/NAMc6-10.h5 50000 /tmp/fetchmarkerlist.out
# bin/fetchmarkerlist_rand /local/data/NAM_HM32/NAMc6-10.h5 100000 /tmp/fetchmarkerlist.out
# bin/fetchmarkerlist_rand /local/data/NAM_HM32/NAMc6-10.h5 500000 /tmp/fetchmarkerlist.out
# bin/fetchmarkerlist_rand /local/data/NAM_HM32/NAMc6-10.h5 1000000 /tmp/fetchmarkerlist.out
# bin/fetchmarkerlist_rand /local/data/NAM_HM32/NAMc6-10.h5 5000000 /tmp/fetchmarkerlist.out


# Case 3, M markers for N samples
# Contiguous
# bin/haploblock /local/data/NAM_HM32/NAMc6-10.h5 1000 1000
# bin/haploblock /local/data/NAM_HM32/NAMc6-10.h5 1000 3000
# bin/haploblock /local/data/NAM_HM32/NAMc6-10.h5 1000 4000
# bin/haploblock /local/data/NAM_HM32/NAMc6-10.h5 50000 1000
# bin/haploblock /local/data/NAM_HM32/NAMc6-10.h5 50000 3000
# bin/haploblock /local/data/NAM_HM32/NAMc6-10.h5 50000 4000
# bin/haploblock /local/data/NAM_HM32/NAMc6-10.h5 1000000 1000
# bin/haploblock /local/data/NAM_HM32/NAMc6-10.h5 1000000 3000
# bin/haploblock /local/data/NAM_HM32/NAMc6-10.h5 1000000 4000

# Random
bin/haplarray_rand 1000 1000
bin/haplarray_rand 1000 3000
bin/haplarray_rand 1000 4000
bin/haplarray_rand 50000 1000
bin/haplarray_rand 50000 3000
bin/haplarray_rand 50000 4000
bin/haplarray_rand 1000000 1000
bin/haplarray_rand 1000000 3000
bin/haplarray_rand 1000000 4000

