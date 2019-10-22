# HDF5

The .c files in this directory were compiled with "h5cc -O" to produce the executables in the bin/ subdirectory.
These programs were then executed with appropriate arguments using the script ./runtests.csh, and logged the
results to the .log file for each test case:
- "Case 1: All markers for N samples": samplestest.log 
- "Case 2: All samples for N markers": markerstest.log 
- "Case 3: N markers x M samples" : haplotest.log

The programs used for the test cases were:
- Case 1 Contiguous: fetchsampleblock
- Case 1 Random: fetchsamplelist_test
- Case 2 Contiguous: fetchmarkerblock
- Case 2 Random: fetchmarkerlist_rand
- Case 3 Contiguous: haploblock
- Case 3 Random: haplarray_rand

Results for repeated tests were very consistent, except for Case 2 and 3 Random.  For these cases, some runs
were suspiciously fast and suggested the possibility of data caching in the system.  To prevent this, the
previous programs fetchmarkerlist and haplarray were modified to create a new random list of marker positions
and sample positions with each run.

The HDF5 datafile used for these tests, /local/data/NAM_HM32/NAMc6-10.h5, was created with program loadNAMc6-10
from the csv file /local/data/NAM_H32/csv/c6-10.csv, which was subsequently moved to /share/NAM_HM32/csv/c6-10.csv.
c6-10.csv is the catenation of files c6.csv, c7.csv, .. c10.csv with the header line removed.


