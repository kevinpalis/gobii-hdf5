#!/bin/sh

# Run the HDF5 timing tests.

# Case 2, all samples for a specified number of markers.
# SNPs
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.10k ./fetchmarkerlist.out
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.100k ./fetchmarkerlist.out
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist markers-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.5m ./fetchmarkerlist.out
# Use the latest program:
# bin/fetchmarkerlist_indel_lkp_sf markers-fast ../data/generated_dataset_snp.h5 empty ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf markers-fast ../data/generated_dataset_snp.h5 empty ../randomlists/h5_markers.10k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf markers-fast ../data/generated_dataset_snp.h5 empty ../randomlists/h5_markers.100k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf markers-fast ../data/generated_dataset_snp.h5 empty ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf markers-fast ../data/generated_dataset_snp.h5 empty ../randomlists/h5_markers.5m ./fetchmarkerlist.out
# bin/fetchmarkerlist samples-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
# bin/fetchmarkerlist samples-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.10k ./fetchmarkerlist.out
# bin/fetchmarkerlist samples-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.100k ./fetchmarkerlist.out
# bin/fetchmarkerlist samples-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist samples-fast ../data/generated_dataset_snp.h5 ../randomlists/h5_markers.5m ./fetchmarkerlist.out
# bin/fetchmarkerblock ../data/generated_dataset_snp.h5 7000017 1000 ./fetchmarkerblock.out
# bin/fetchmarkerblock ../data/generated_dataset_snp.h5 8000018 10000 ./fetchmarkerblock.out
# bin/fetchmarkerblock ../data/generated_dataset_snp.h5 9000019 100000 ./fetchmarkerblock.out
# bin/fetchmarkerblock ../data/generated_dataset_snp.h5 10000020 1000000 ./fetchmarkerblock.out
# bin/fetchmarkerblock ../data/generated_dataset_snp.h5 11000031 5000000 ./fetchmarkerblock.out

# Indels100 lookup contiguous
# bin/fetchmarkerblock_indel_lkp2 ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 7000017 1000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp2 ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 8000018 10000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp2 ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 9000019 100000 ./fetchmarkerblock.out
# ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp2 ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 10000020 1000000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp2 ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 11000031 5000000 ./fetchmarkerblock.out

# Indels100 lookup random markers
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.10k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.100k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.5m ./fetchmarkerlist.out
# Indels100 lookup random markers, transposed:
# bin/fetchmarkerlist_indel_lkp_sf samples-fast ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf samples-fast ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.10k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf samples-fast ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.100k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf samples-fast ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf samples-fast ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_markers.5m ./fetchmarkerlist.out

# Tetraploid random markers
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.10k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.100k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.5m ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lk
# Tetraploid random markers, transposed:
# bin/fetchmarkerlist_indel_lkp_sf samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.10k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.100k ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.5m ./fetchmarkerlist.out
# Tetraploid random markers, transposed, transposing the output back to Hapmap format:
# bin/fetchmarkerlist_indel_lkp_sf2_tp_batch samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.1000 fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf2_tp_batch samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.10k fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf2_tp_batch samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.100k fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf2_tp_batch samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.1m fetchmarkerlist.out
# bin/fetchmarkerlist_indel_lkp_sf2_tp_batch samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.5m fetchmarkerlist.out

# Tetraploid contiguous markers
# bin/fetchmarkerblock_indel_lkp2 ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts 7000017 1000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp2 ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts 8000018 10000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp2 ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts 9000019 100000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp2 ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts 10000020 1000000 ./fetchmarkerblock.out
# bin/fetchmarkerblock_indel_lkp2 ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts 11000031 5000000 ./fetchmarkerblock.out

# Indels100 padded markers
# bin/fetchmarkerblock_pad ../data/generated_dataset_indels100.pad.h5 7000017 1000 ./fetchmarkerblock_pad.out
# bin/fetchmarkerblock_pad ../data/generated_dataset_indels100.pad.h5 8000018 10000 ./fetchmarkerblock_pad.out
# bin/fetchmarkerblock_pad ../data/generated_dataset_indels100.pad.h5 9000019 100000 ./fetchmarkerblock_pad.out
# bin/fetchmarkerblock_pad ../data/generated_dataset_indels100.pad.h5 10000020 1000000 ./fetchmarkerblock_pad.out
# bin/fetchmarkerblock_pad ../data/generated_dataset_indels100.pad.h5 11000031 5000000 ./fetchmarkerblock.out
# bin/fetchmarkerlist_pad markers-fast ../data/generated_dataset_indels100.pad.h5  ../randomlists/h5_markers.1000 ./fetchmarkerlist.out
# bin/fetchmarkerlist_pad markers-fast ../data/generated_dataset_indels100.pad.h5  ../randomlists/h5_markers.10k ./fetchmarkerlist.out
# bin/fetchmarkerlist_pad markers-fast ../data/generated_dataset_indels100.pad.h5  ../randomlists/h5_markers.100k ./fetchmarkerlist.out
# bin/fetchmarkerlist_pad markers-fast ../data/generated_dataset_indels100.pad.h5  ../randomlists/h5_markers.1m ./fetchmarkerlist.out
# bin/fetchmarkerlist_pad markers-fast ../data/generated_dataset_indels100.pad.h5  ../randomlists/h5_markers.5m ./fetchmarkerlist.out

# Case 1, all markers for a specified number of samples
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 4000 50 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 4100 100 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 4200 500 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 3000 1000 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 1900 2000 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 800 3000 /local/data/NAM_HM32/fetchsampleblock.out
# bin/fetchsampleblock /local/data/NAM_HM32/NAMc6-10.h5 500 4000 /local/data/NAM_HM32/fetchsampleblock.out

# Random, SNPs only, samples
# bin/fetchsamplelist_snp ../data/generated_dataset_snp.h5 ../randomlists/h5_samples.50 50 ./fetchsamplelist.out
# bin/fetchsamplelist_snp ../data/generated_dataset_snp.h5 ../randomlists/h5_samples.100 100 ./fetchsamplelist.out
# bin/fetchsamplelist_snp ../data/generated_dataset_snp.h5 ../randomlists/h5_samples.500 500 ./fetchsamplelist.out
# bin/fetchsamplelist_snp ../data/generated_dataset_snp.h5 ../randomlists/h5_samples.1000 1000 ./fetchsamplelist.out
# bin/fetchsamplelist_snp ../data/generated_dataset_snp.h5 ../randomlists/h5_samples.2000 2000 ./fetchsamplelist.out
# bin/fetchsamplelist_snp ../data/generated_dataset_snp.h5 ../randomlists/h5_samples.3000 3000 ./fetchsamplelist.out

# Random, Indels padded, samples
# bin/fetchsamplelist_pad ../data/generated_dataset_indels100.pad.h5 ../randomlists/h5_samples.50 50 ./fetchsamplelist.out
# bin/fetchsamplelist_pad ../data/generated_dataset_indels100.pad.h5 ../randomlists/h5_samples.100 100 ./fetchsamplelist.out
# bin/fetchsamplelist_pad ../data/generated_dataset_indels100.pad.h5 ../randomlists/h5_samples.500 500 ./fetchsamplelist.out
# bin/fetchsamplelist_pad ../data/generated_dataset_indels100.pad.h5 ../randomlists/h5_samples.1000 1000 ./fetchsamplelist.out
# bin/fetchsamplelist_pad ../data/generated_dataset_indels100.pad.h5 ../randomlists/h5_samples.2000 2000 ./fetchsamplelist.out
# bin/fetchsamplelist_pad ../data/generated_dataset_indels100.pad.h5 ../randomlists/h5_samples.3000 3000 ./fetchsamplelist.out

# Random, Indels lookup, samples
# bin/fetchsamplelist_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_samples.50 50 ./fetchsamplelist.out
# bin/fetchsamplelist_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_samples.100 100 ./fetchsamplelist.out
# bin/fetchsamplelist_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_samples.500 500 ./fetchsamplelist.out
# bin/fetchsamplelist_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_samples.1000 1000 ./fetchsamplelist.out
# bin/fetchsamplelist_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_samples.2000 2000 ./fetchsamplelist.out
# bin/fetchsamplelist_lkp ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_samples.3000 3000 ./fetchsamplelist.out

# Case 3, M markers for N samples
# Contiguous
# SNPs only
# bin/haploblock_snp ../data/generated_dataset_snp.h5 1000 1000
# bin/haploblock_snp ../data/generated_dataset_snp.h5 50000 1000
# bin/haploblock_snp ../data/generated_dataset_snp.h5 50000 2000
# bin/haploblock_snp ../data/generated_dataset_snp.h5 50000 3000
# bin/haploblock_snp ../data/generated_dataset_snp.h5 1000000 1000
# bin/haploblock_snp ../data/generated_dataset_snp.h5 1000000 2000
# bin/haploblock_snp ../data/generated_dataset_snp.h5 1000000 3000
# Indels
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 1000 1000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 50000 1000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 50000 2000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 50000 3000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 1000000 1000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 1000000 2000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 1000000 3000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 1000 1000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 50000 1000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 50000 3000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 50000 3000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 1000000 1000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 1000000 2000
bin/haploblock_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 1000000 3000

# Random
# SNPs only
# bin/haplarray_rand_snp ../data/generated_dataset_snp.h5 ../randomlists/SNPs_only/h5_markers.1000 ../randomlists/SNPs_only/h5_samples.1000
# bin/haplarray_rand_snp ../data/generated_dataset_snp.h5 ../randomlists/SNPs_only/h5_markers.50k ../randomlists/SNPs_only/h5_samples.1000
# bin/haplarray_rand_snp ../data/generated_dataset_snp.h5 ../randomlists/SNPs_only/h5_markers.50k ../randomlists/SNPs_only/h5_samples.2000
# bin/haplarray_rand_snp ../data/generated_dataset_snp.h5 ../randomlists/SNPs_only/h5_markers.50k ../randomlists/SNPs_only/h5_samples.3000
# bin/haplarray_rand_snp ../data/generated_dataset_snp.h5 ../randomlists/SNPs_only/h5_markers.1m ../randomlists/SNPs_only/h5_samples.1000
# bin/haplarray_rand_snp ../data/generated_dataset_snp.h5 ../randomlists/SNPs_only/h5_markers.1m ../randomlists/SNPs_only/h5_samples.2000
# bin/haplarray_rand_snp ../data/generated_dataset_snp.h5 ../randomlists/SNPs_only/h5_markers.1m ../randomlists/SNPs_only/h5_samples.3000
# Indels
# bin/haplarray_rand_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.1000 ../randomlists/SNPs_only/h5_samples.1000
# bin/haplarray_rand_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.50k ../randomlists/SNPs_only/h5_samples.1000
# bin/haplarray_rand_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.50k ../randomlists/SNPs_only/h5_samples.2000
# bin/haplarray_rand_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.50k ../randomlists/SNPs_only/h5_samples.3000
# bin/haplarray_rand_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.1m ../randomlists/SNPs_only/h5_samples.1000
# bin/haplarray_rand_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.1m ../randomlists/SNPs_only/h5_samples.2000
# bin/haplarray_rand_indel ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../ra
