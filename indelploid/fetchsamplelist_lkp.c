/* fetchsamplelist_lkp.c, 23jan20, from:
   fetchsamplelist_snp.c, 21jan2020, from:
   fetchsamplelist_test.c, 24apr2017, from fetchsamplelist.c, 8jun2016, 
   from fetchsample.c, 7may16, from: samplestest.c, 26jan2015
   Fetch a set of entire rows, alleles for all markers for the specified samples. */
/* 12jun16, Detect and handle different datatypes. */
/* 24apr17, Add parameter to query only the first n samples from the list. */
/* 21jan17, DON'T handle missing samples ("-1"). */
/* 23jan20, Handle insertions, specified in separate text file *.h5.inserts. */

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define DATASETNAME "/allelematrix_samples-fast" 
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
  int datumsize, i, j, k;

  if (argc < 5) {
    printf("Usage: %s <HDF5 file> <insertsfile> <queryfile> <samplecount> <output file>\n", argv[0]);
    printf("E.g. %s ../data/generated_dataset_indels100.lkp.h5 ../data/generated_dataset_indels100.lkp.h5.inserts ../randomlists/h5_samples.50 50 ./fetchsamplelist.out\n", argv[0]);
    printf("Fetch alleles for the first samplecount samples listed in queryfile, for all markers.\n");
    printf("The queryfile contains sample position numbers, separated by newline.\n");
    printf("The insertsfile contains the insertion sequences for indel markers.\n");
    return 0;
  }

  /* Start the timer. */
  time_t starttime = time(NULL);

  /* Read the arguments. */
  char *h5filename = argv[1];
  char *insertsfilename = argv[2];
  char *queryfilename = argv[3];
  int samplecount = atoi(argv[4]);
  char *outfilename = argv[5];

  outfile = fopen (outfilename, "w");

  /* Write a log of the results. */
  FILE *logfile = fopen ("samplestest.log", "a");
  fprintf(logfile, "Indels, lookup table. Number of samples: %i\n", samplecount);
  fprintf(logfile, "Mode: random\n");

  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (h5filename, H5F_ACC_RDONLY, H5P_DEFAULT);
  if (file_id < 0) {
    printf("Failure opening input file %s\n", h5filename);
    return 1;
  }
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);
  dataspace_id = H5Dget_space (dataset_id);

  /* Find the dimensions of the HDF5 file dataset. */
  H5Sget_simple_extent_dims(dataspace_id, filedims, NULL);
  int SampleTotal = filedims[0];
  int MarkerTotal = filedims[1];

  /* Determine the datatype and the size of an individual element. */
  datumtype = H5Dget_type(dataset_id);
  datumsize = H5Tget_size(datumtype);

  // To dimension the query array, how many samples are in queryfile?
  FILE *queryfile = fopen(queryfilename, "r");
  char *row = malloc(200);  // Used for insertsfile too.
  int sampleCount = 0;
  while (fgets (row, 10, queryfile) != NULL)
    sampleCount++;
  fclose(queryfile);
  int *samples = malloc(sampleCount * sizeof(int));

  /* Read in the list of queried sample positions into array samples[]. */
  queryfile = fopen (queryfilename, "r");
  char *listitem = malloc(10);
  i = 0;
  while (fgets(listitem, 10, queryfile) != NULL) {
    samples[i] = atoi(listitem);
    if (samples[i] > SampleTotal) {
      printf("Sample number %i out of range.\n", samples[i]);
      return 1;
    }
    i++;
  }
  fclose(queryfile);

  /* To dimension the inserts arrays, how many markers are in insertsfile? */
  FILE *insertsfile = fopen(insertsfilename, "r");
  int insertCount = 0;
  while (fgets(row, 200, insertsfile) != NULL) 
    insertCount++;
  fclose (insertsfile);

  /* Read the insertion sequences and their marker numbers into memory arrays. */
  insertsfile = fopen(insertsfilename, "r");
  int *markernums = malloc(insertCount * sizeof(int));
  char **seqs = malloc(insertCount * sizeof(char *));
  for (i = 0; i < insertCount; i++) {
    seqs[i] = malloc(110);  // Max length of insertion sequence is 100 bp.
    if (seqs[i] == NULL) {
      printf("malloc seqs[i] failed, i = %i\n", i);
      return 1;
    }
  }
  i = 0;
  while (fgets(row, 200, insertsfile) != NULL) {
    markernums[i] = atoi(strtok(row, "\t"));
    strcpy(seqs[i], strtok(NULL, "\t\n"));
    i++;
  }

  /* Set up the HDF5 file parameters. */
  /* Create memory space with size of subset. Get file dataspace. */
  dimsm[0] = 1;
  dimsm[1] = MarkerTotal; 
  char *rdata = malloc(MarkerTotal * datumsize);  /* buffer for read */
  memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
  /* Select subset from file dataspace. */
  start[1] = 0;
  stride[0] = 1; stride[1] = 1;
  count[0] = 1; count[1] = 1;
  block[0] = 1; block[1] = MarkerTotal;
  
  /* Read the array of sample positions one at a time, outputting the matrix row for each. */
  int samplenum = 1;
  for (i = 0; i < sampleCount; i++) {  
    start[0] = samples[i];
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    /* Read the hyperslab. */
    status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
    /* Write the results to the output file, as a tab-delimited string for each sample. */
    int indel = 0; // Which indel from insertsfile are we looking at?
    for (j = 0; j < MarkerTotal * datumsize; j = j + datumsize) {
      for (k = 0; k < datumsize; k++) 
	// If the allele is an insertion, look up the sequence.
	if (rdata[j + k] == '1') {
	  while (markernums[indel] < j/datumsize)
	    indel++;
	  fprintf(outfile, "%s", seqs[indel]);
	}
	else
	  if (rdata[j + k] != '\0')  // Chop off the trailing <null> character.
	    fprintf(outfile, "%c", rdata[j + k]);
      /* No trailing <Tab> at end of line. */
      if (j < (MarkerTotal - 1) * datumsize)
	fprintf(outfile, "\t");
    }
    fprintf(outfile, "\n");
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
