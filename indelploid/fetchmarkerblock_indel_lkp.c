/* fetchmarkerblock_indel_lkp.c, 13nov19, from: */
/* fetchmarkerblock.c, 26apr17, from fetchmarkerlist_test.c, 26apr17, from fetchmarkerlist.c, 8jun16
   Fetch a set of entire rows, alleles for all samples for the specified contiguous block  of markers. */
/* 13jun16, Detect and handle different datatypes. */
/* 7dec16, Return a row of "N"s if marker number is -1. */
/* 13nov19, Handle insertions, specified in separate text file *.h5.inserts. */

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
  hsize_t     filedims[RANK], dimsm[RANK];   
  hsize_t     start[RANK], stride[RANK], count[RANK], block[RANK];
  hid_t       file_id, dataset_id;        /* handles */
  hid_t       dataspace_id, memspace_id; 
  hid_t       datumtype;
  herr_t      status;                   

  FILE *outfile;
  int datumsize, j, k;

  if (argc < 6) {
    printf("Usage: %s <HDF5 file> <inserts file> <firstmarker> <markercount> <output file>\n", argv[0]);
    printf("E.g. %s /data/polyploid_indel_benchmarking/generated_dataset_indels100.lkp.h5 /data/polyploid_indel_benchmarking/generated_dataset_indels100.lkp.h5.inserts 5000017 1000 ./fetchmarkerblock.out\n", argv[0]);
    printf("Fetch alleles for markercount markers beginning with firstmarker, for all samples.\n");
    return 0;
  }
  /* Read the arguments. */
  char *h5filename = argv[1];
  char *lookuplist = argv[2];;
  int firstmarker = atoi(argv[3]);
  int markercount = atoi(argv[4]);
  char *outfilename = argv[5];
  outfile = fopen (outfilename, "w");
  char *row = malloc(5000);

  FILE *inserts;    /* text file of insert sequences */

  /* Write a log of the results. */
  FILE *logfile = fopen ("markerstest.log", "a");
  fprintf(logfile, "Number of markers: %i, indels in lookup file\n", markercount);
  fprintf(logfile, "Mode: contiguous\n");

  time_t starttime = time(NULL);

  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (h5filename, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);
  dataspace_id = H5Dget_space (dataset_id);

  /* Find the dimensions of the HDF5 file dataset. */
  H5Sget_simple_extent_dims(dataspace_id, filedims, NULL);
  int MarkerTotal = filedims[0];
  int SampleTotal = filedims[1];

  if (firstmarker + markercount > MarkerTotal) {
    printf("There are only %i markers in the dataset.\n", MarkerTotal);
    return 1;
    }

  /* Determine the datatype and the size of an individual element. */
  datumtype = H5Dget_type(dataset_id);
  datumsize = H5Tget_size(datumtype);

  /* Create memory space with size of subset. Get file dataspace. */
  dimsm[0] = 1;
  dimsm[1] = SampleTotal; 
  char *rdata = malloc(SampleTotal * datumsize);  /* buffer for read */
  memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
  /* Select subset from file dataspace. */
  start[1] = 0;
  stride[0] = 1; stride[1] = 1;
  count[0] = 1; count[1] = 1;
  block[0] = 1; block[1] = SampleTotal;

  int marker;
  for (marker = firstmarker; marker < firstmarker+markercount; marker++) {
    /* Read and output the alleles. */
    start[0] = marker;
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    /* Read the hyperslab. */
    status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
    /* Write the results to the output file, as a tab-delimited string for each marker. */
    char *token = malloc(200);
    char *sequence = malloc(200);
    int markerindex = -1;
    bool lookedup = false;
    char c;
    for (j = 0; j < SampleTotal * datumsize; j = j + datumsize) {
      for (k = 0; k < datumsize; k++) {
	// If the allele is an insertion, look up the sequence.
	if (rdata[j + k] == '1') {
	  if (lookedup == false) {  // first encounter with this marker on this row of input file
	    // Look it up in the *.h5.inserts file.
	    // todo: Load the inserts file into a memory array instead of reading it repeatedly.
	    inserts = fopen(lookuplist, "r");
	    while (markerindex < marker) {
	      row = fgets(row, 5000, inserts);
	      token = strtok(row, "\t");
	      markerindex = atoi(token);
	    }
	    sequence = strtok(NULL, "\t\n");  // Extract the sequence.
	    lookedup = true;
	    fclose(inserts);  // Close and reopen to search next time from the top.
	    inserts = fopen(lookuplist, "r");
	  }
	  for (c=0; c<strlen(sequence); c++)
	    fprintf(outfile, "%c", sequence[c]);
	}
	else
	  if (rdata[j + k] != '\0')  // Chop off the trailing <null> character.
	    fprintf(outfile, "%c", rdata[j + k]);
      }
      /* No trailing <Tab> at end of line. */
      if (j < (SampleTotal - 1) * datumsize)
	fprintf(outfile, "\t");
    }
    fprintf(outfile, "\n");
    lookedup = false;
  }
  /* We're done. Exit*/ 
  fclose(outfile);
  time_t stoptime = time(NULL);
  int elapsed = stoptime - starttime;
  fprintf(logfile, "Elapsed time: %i seconds\n\n", elapsed);
  fclose(logfile);
  status = H5Tclose(datumtype);
  status = H5Sclose (memspace_id);
  status = H5Sclose (dataspace_id);
  status = H5Dclose (dataset_id);
  status = H5Fclose (file_id);
  if (status >= 0) return 0;
  else return 1;
}
