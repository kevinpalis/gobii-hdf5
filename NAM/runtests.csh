#!/bin/csh

# Run the HDF5 timing tests.

# Case 1, all markers for a specified number of samples.
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 50 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 100 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 500 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 1000 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 2000 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 3000 /local/data/NAM_HM32/randomsamples.out
# bin/fetchsamplelist_test /local/data/NAM_HM32/NAMc6-10.h5 samplelist 4000 /local/data/NAM_HM32/randomsamples.out

#bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 4000 50 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 4100 100 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 4200 500 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 3000 1000 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 1900 2000 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 800 3000 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 500 4000 /local/data/NAM_HM32/fetchsampleblock.out

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
bin/fetchmarkerlist_test /local/data/NAM_HM32/NAMc6-10.h5 ./markerlist 1000 /tmp/fetchmarkerlist.out
bin/fetchmarkerlist_test /local/data/NAM_HM32/NAMc6-10.h5 ./markerlist 5000 /tmp/fetchmarkerlist.out
bin/fetchmarkerlist_test /local/data/NAM_HM32/NAMc6-10.h5 ./markerlist 10000 /tmp/fetchmarkerlist.out
bin/fetchmarkerlist_test /local/data/NAM_HM32/NAMc6-10.h5 ./markerlist 50000 /tmp/fetchmarkerlist.out
bin/fetchmarkerlist_test /local/data/NAM_HM32/NAMc6-10.h5 ./markerlist 100000 /tmp/fetchmarkerlist.out
bin/fetchmarkerlist_test /local/data/NAM_HM32/NAMc6-10.h5 ./markerlist 500000 /tmp/fetchmarkerlist.out
bin/fetchmarkerlist_test /local/data/NAM_HM32/NAMc6-10.h5 ./markerlist 1000000 /tmp/fetchmarkerlist.out
bin/fetchmarkerlist_test /local/data/NAM_HM32/NAMc6-10.h5 ./markerlist 5000000 /tmp/fetchmarkerlist.out


# Case 3, M markers for N samples
# Contiguous
# haploblock 1000 1000
# haploblock 3000 3000
# haploblock 30000 3000

# Random
# haplotest.pl 1000 1000
# haplotest.pl 3000 3000
# haplotest.pl 10000 1000
# haplotest.pl 30000 3000
