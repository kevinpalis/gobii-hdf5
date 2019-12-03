#!/bin/sh

# Run the HDF5 timing tests.

# Case 2, all samples for a specified number of markers.
# SNPs
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.10k ./fetchmarkerlist.out
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.100k ./fetchmarkerlist.out
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.5m ./fetchmarkerlist.out
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.5m ./fetchmarkerlist.out
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.5m ./fetchmarkerlist.out
# bin/fetchmarkerblock ../data/generated_dataset_snp.h5 7000017 1000 ./fetchmarkerblock.out
# bin/fetchmarkerblock ../data/generated_dataset_snp.h5 8000018 10000 ./fetchmarkerblock.out
# bin/fetchmarkerblock ../data/generated_dataset_snp.h5 9000019 100000 ./fetchmarkerblock.out
# bin/fetchmarkerblock ../data/generated_dataset_snp.h5 10000020 1000000 ./fetchmarkerblock.out
# bin/fetchmarkerblock ../data/generated_dataset_snp.h5 11000031 5000000 ./fetchmarkerblock.out

# Indels100 lookup
# bin/fetchmarkerblock_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 7000017 1000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 8000018 10000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 9000019 100000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 10000020 1000000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 11000031 5000000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 11000031 5000000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 11000031 5000000 ./fetchmarkerblock.out

# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.10k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.100k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.5m ./fetchmarkerlist.out

# Indels100 padded
# bin/fetchmarkerblock_pad ../data/generated_dataset_indels100.pad.h5 7000017 1000 ./fetchmarkerblock_pad.out
# bin/fetchmarkerblock_pad ../data/generated_dataset_indels100.pad.h5 8000018 10000 ./fetchmarkerblock_pad.out
# bin/fetchmarkerblock_pad ../data/generated_dataset_indels100.pad.h5 9000019 100000 ./fetchmarkerblock_pad.out
# bin/fetchmarkerblock_pad ../data/generated_dataset_indels100.pad.h5 10000020 1000000 ./fetchmarkerblock_pad.out
# bin/fetchmarkerblock_pad ../data/generated_dataset_indels100.pad.h5 11000031 5000000 ./fetchmarkerblock.out
bin/fetchmarkerlist_pad markers-fast ../data/generated_dataset_indels100.pad.h5  ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
bin/fetchmarkerlist_pad markers-fast ../data/generated_dataset_indels100.pad.h5  ../randomlists/h5_markers.10k ./fetchmarkerlist.out
bin/fetchmarkerlist_pad markers-fast ../data/generated_dataset_indels100.pad.h5  ../randomlists/h5_markers.100k ./fetchmarkerlist.out
bin/fetchmarkerlist_pad markers-fast ../data/generated_dataset_indels100.pad.h5  ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist_pad markers-fast ../data/generated_dataset_indels100.pad.h5  ../randomlists/h5_markers.5m ./fetchmarkerlist.out

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
# bin/haplarray_rand 1000 1000
# bin/haplarray_rand 1000 3000
# bin/haplarray_rand 1000 4000
# bin/haplarray_rand 50000 1000
# bin/haplarray_rand 50000 3000
# bin/haplarray_rand 50000 4000
# bin/haplarray_rand 1000000 1000
# bin/haplarray_rand 1000000 3000
# bin/haplarray_rand 1000000 4000

