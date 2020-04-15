/* haploblock_indel.c, 14apr2020, from:
/* haploblock_snp.c, 11apr2020, from:
/* haploblock.c, 16feb2015, from samplestest.c, 26jan2015
   Fetch a contiguous block of rows and columns from a two-dimensional HDF5 file. */

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

#define DATASETNAME "/allelematrix" 
#define RANK  2                           /* number of dimensions */

int main (int argc, char *argv[]) {

  /* HDF5 variables */
  hsize_t     dimsm[RANK];   
  hsize_t     start[RANK], stride[RANK], count[RANK], block[RANK];
  hid_t       file_id, dataset_id;        /* handles */
  hid_t       dataspace_id, memspace_id; 
  herr_t      status;                   

  FILE *outfile;
  FILE *logfile;
  int i, j, k, s;
  time_t starttime, stoptime;
  int elapsed;

  if (argc < 4) {
    printf("Usage: %s <HDF5 file> <insertfile> <number of markers> <number of samples>\n", argv[0]);
    printf("Example: %s ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts 100 100\n", argv[0]);
    printf("Output is in ./haplotest.out and ./haplotest.log.\n");
    return 0;
  }
  /* Read the arguments. */
  char *H5FILE = argv[1];
  char *insertsfilename = argv[2];
  int markertotal = atoi(argv[3]);
  int sampletotal = atoi(argv[4]);

  /* Write a log of the results. */
  logfile = fopen ("haplotest.log", "a");
  fprintf(logfile, "%s\n", argv[0]);
  fprintf(logfile, "Fetching allele calls for %i markers and %i samples.\n", markertotal, sampletotal);
  fprintf(logfile, "Mode: contiguous\n");

  starttime = time(NULL);

  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (H5FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);

  /* Determine the datatype and the size of an individual element. */
  hid_t datumtype = H5Dget_type(dataset_id);
  int datumsize = H5Tget_size(datumtype);

  /* First row of the region. Leave room for the max size of the region. */
  int firstmarker = rand() % 18000000;
  /* First column of the region */
  int firstsample;
  if (sampletotal == 3000)
    firstsample = 0;
  else
    firstsample = rand() % 900;

  /* To dimension the inserts arrays, how many markers are in insertsfile? */
  FILE *insertsfile = fopen(insertsfilename, "r");
  char row[200];
  int insertCount = 0;
  while (fgets(row, 200, insertsfile) != NULL)
    insertCount++;
  fclose (insertsfile);

  /* Read the insertion sequences and their marker numbers into memory arrays. */
  insertsfile = fopen(insertsfilename, "r");
  int *markernums = malloc(insertCount * sizeof(int));
  // seqs[] holds the array of insert sequences.
  char **seqs = malloc(insertCount * sizeof(char *));
  for (i = 0; i < insertCount; i++) 
    seqs[i] = malloc(110);  // Max length of insertion sequence is 100 bp.
  i = 0;
  while (fgets(row, 200, insertsfile) != NULL) {
    // markernums[] holds the array of which marker numbers (positions in the matrix) have inserts.
    markernums[i] = atoi(strtok(row, "\t"));
    strcpy(seqs[i], strtok(NULL, "\t\n"));
    i++;
  }

  /* Create memory space with size of subset. Get file dataspace. */
  dimsm[0] = 1;
  /* dimsm[1] = sampletotal * datumsize; */
  dimsm[1] = sampletotal;
  memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
  dataspace_id = H5Dget_space (dataset_id);
  char rdata[sampletotal * datumsize];
  remove("./haplotest.out");
  outfile = fopen ("./haplotest.out", "a");
  for (j = 0; j < markertotal; ++j) {
    /* Select subset from file dataspace. */
    start[0] = firstmarker + j; start[1] = firstsample;
    stride[0] = 1; stride[1] = 1;
    count[0] = 1; count[1] = 1;
    block[0] = 1; block[1] = sampletotal;
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    /* Read the hyperslab, one row. */
    status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);

    /* Write the results to the output file, as a tab-delimited string for each marker. */
    bool lookedup = false;
    for (i = 0; i < sampletotal; i++) {
      for (k = 0; k < datumsize; k++)
	// If the allele is an insertion, look up the sequence.
	if (rdata[i*datumsize + k] == '1') {
	  if (lookedup == false) {  // first encounter with this marker on this row of input file
	    // Look it up in the inserts arrays and output it. 
	    s = 0;
	    /* while (markernums[s] < markers[i]) */
	    while (markernums[s] < firstmarker + j)
	      s++;
	    lookedup = true;
	  }
	  fprintf(outfile, "%s", seqs[s]);
	}
	else
	  if (rdata[i*datumsize + k] != '\0')  // Chop off the trailing <null> character.
	    fprintf(outfile, "%c", rdata[i*datumsize + k]);
      fprintf(outfile, "\t");
    }
    fprintf(outfile, "\n");
  }

  fclose(outfile);
  status = H5Sclose (memspace_id);
  status = H5Sclose (dataspace_id);

  stoptime = time(NULL);
  elapsed = stoptime - starttime;
  fprintf(logfile, "Elapsed time: %i seconds\n\n", elapsed);
  fclose(logfile);

  status = H5Dclose (dataset_id);
  status = H5Fclose (file_id);
  if (status) return 0;
  else return 1;
}
