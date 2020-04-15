/* fetchmarkerlist_indel_lkp_sf2.c, 2mar20, from: */
/* fetchmarkerlist_indel_lkp_sf.c, 22feb20, from: */
/* fetchmarkerlist_indel_lkp.c, 21nov19, from: */
/* fetchmarkerlist.c, 13nov19, from: */
/* fetchmarkerlist.c, 8jun16, from fetchmarker.c, 7may16 from: fetchsample.c, 7may16
   Fetch a set of entire rows, alleles for all samples for the specified list of markers. */
/* 13jun16, Detect and handle different datatypes. */
/* 7dec16, Return a row of "N"s if marker number is -1. */
/* 7dec16, Take the list of markers from a file instead of command arguments. */
/* 4may17, Output in either orientation. */
/* 13nov19, Write execution times to a log. */
/* 21nov19, Handle insertions, specified in separate text file *.h5.inserts. 
            Output only markers-fast. */
/* 22feb20  Allow samples-fast too. */
/* 2mar20   Correct bug in samples-fast (returning _all_ markers instead of markers[i]). */

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#define RANK  2                           /* number of dimensions */

int main (int argc, char *argv[]) {

  /* HDF5 variables */
  hsize_t     filedims[RANK], dimsm[RANK];   
  hsize_t     start[RANK], stride[RANK], count[RANK], block[RANK];
  hid_t       file_id, dataset_id;        /* handles */
  hid_t       dataspace_id, memspace_id; 
  hid_t       datumtype;
  herr_t      status;                   

  char *h5dataset;
  int datumsize, i, j, k, s, c;
  bool lookedup = false;

  if (argc < 6) {
    printf("Usage: %s <orientation> <HDF5 file> <insertsfile> <queryfile> <output file>\n", argv[0]);
    printf("E.g. %s samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/h5_markers.1000 fetchmarkerlist.out\n", argv[0]);
    printf("Fetch alleles for the markers listed in the queryfile, for all samples.\n");
    printf("The queryfile contains marker position numbers, separated by newline.\n");
    printf("The insertsfile contains the insertion sequences for indel markers.\n");
    return 0;
  }
  /* Read the arguments. */
  if (strcmp(argv[1], "samples-fast") == 0)
    h5dataset = "/allelematrix_samples-fast";
  else
    h5dataset = "/allelematrix";
  char *h5filename = argv[2];
  char *insertsfilename = argv[3];
  char *queryfilename = argv[4];
  char *outfilename = argv[5];

  FILE *outfile = fopen (outfilename, "w");

  /* Write a log of the results. */
  FILE *logfile = fopen ("markerstest.log", "a");

  time_t starttime = time(NULL);

  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (h5filename, H5F_ACC_RDONLY, H5P_DEFAULT);
  if (file_id < 0) {
    printf("Failure opening input file %s\n", h5filename);
    return 1;
  }
  dataset_id = H5Dopen2 (file_id, h5dataset, H5P_DEFAULT);
  dataspace_id = H5Dget_space (dataset_id);

  /* Find the dimensions of the HDF5 file dataset. */
  H5Sget_simple_extent_dims(dataspace_id, filedims, NULL);
  int vertTotal = filedims[0];
  int horizTotal = filedims[1];
  int markerTotal;
  if (strcmp(argv[1], "samples-fast") == 0) 
    markerTotal = horizTotal;
  else
    markerTotal = vertTotal;
  
  /* Determine the datatype and the size of an individual element. */
  datumtype = H5Dget_type(dataset_id);
  datumsize = H5Tget_size(datumtype);

  // To dimension the query array, how many markers are in queryfile?
  FILE *queryfile = fopen(queryfilename, "r");
  char *row = malloc(200);  // Used for insertsfile too.
  int markerCount = 0;
  while (fgets (row, 10, queryfile) != NULL)
    markerCount++;
  fclose(queryfile);
  int *markers = malloc(markerCount * sizeof(int));

  /* Read in the list of queried marker positions into array markers[]. */
  queryfile = fopen (queryfilename, "r");
  char *listitem = malloc(10);
  i = 0;
  while (fgets(listitem, 10, queryfile) != NULL) {
    markers[i] = atoi(listitem);
    if (markers[i] > markerTotal) {
      printf("Marker number %i out of range.\n", markers[i]);
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
  // seqs[] holds the array of insert sequences.
  for (i = 0; i < insertCount; i++) {
    seqs[i] = malloc(110);  // Max length of insertion sequence is 100 bp.
    if (seqs[i] == NULL) {
      printf("malloc seqs[i] failed, i = %i\n", i);
      return 1;
    }
  }
  i = 0;
  while (fgets(row, 200, insertsfile) != NULL) {
    // markernums[] holds the array of which marker numbers (positions in the matrix) have inserts.
    markernums[i] = atoi(strtok(row, "\t"));
    strcpy(seqs[i], strtok(NULL, "\t\n"));
    i++;
  }

  /* Read the HDF5 file. */

  if (strcmp(argv[1], "samples-fast") == 0) {    // "transposed" orientation
    /* Create memory space with size of subset. */
    dimsm[0] = 1;
    dimsm[1] = horizTotal; 
    memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
    /* Select subset from file dataspace. */
    start[1] = 0;
    stride[0] = 1; stride[1] = 1;
    count[0] = 1; count[1] = 1;
    block[0] = 1; block[1] = horizTotal;
    char *rdata = malloc(horizTotal * datumsize);  /* buffer for read */
    int samp; // sample number 
    for (samp = 0; samp < vertTotal; samp++) {
      start[0] = samp;
      status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
      /* Read the hyperslab. */
      status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
      // Print out sideways.  Todo: Transpose to the desired orientation.
      s = 0;  // which position in the array of insert sequences
      for (i = 0; i < markerCount; i++) {
	for (k = 0; k < datumsize; k++)
	  if (rdata[markers[i]*datumsize + k] == '1') { // If it's an insert, look up and output the sequence.
	    while (markernums[s] < markers[i])
	      s++;
	    fprintf(outfile, "%s", seqs[s]);
	  }
          else
            fprintf(outfile, "%c", rdata[markers[i]*datumsize + k]);
	/* No trailing <Tab> at end of line. */
	if (i < markerCount - 1)
	  fprintf(outfile, "\t");
      }
      fprintf(outfile, "\n");
    }
  }

  else {     // Orientation is markers-fast ("normal").
    /* Create memory space with size of subset, one row. */
    dimsm[0] = 1;
    dimsm[1] = horizTotal; 
    memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
    /* Select subset from file dataspace. */
    start[1] = 0;
    stride[0] = 1; stride[1] = 1;
    count[0] = 1; count[1] = 1;
    block[0] = 1; block[1] = horizTotal;
    char *rdata = malloc(horizTotal * datumsize);  /* buffer for read */
    /* Read and output the alleles for each requested marker. */
    for (i = 0; i < markerCount; i++) {
      start[0] = markers[i]; // markers[i] is the number of the current marker, its position in the allelematrix.
      status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
      /* Read the hyperslab. */
      status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
      /* Write the results to the output file, as a tab-delimited string for each row. */
      for (j = 0; j < horizTotal * datumsize; j = j + datumsize) {
	for (k = 0; k < datumsize; k++) 
	  // If the allele is an insertion, look up the sequence.
	  if (rdata[j + k] == '1') {
	    if (lookedup == false) {  // first encounter with this marker on this row of input file
	      // Look it up in the inserts arrays and output it. 
	      s = 0;
	      while (markernums[s] < markers[i])
		s++;
	      lookedup = true;
	    }
	    fprintf(outfile, "%s", seqs[s]);
	  }
	  else
	    if (rdata[j + k] != '\0')  // Chop off the trailing <null> character.
	      fprintf(outfile, "%c", rdata[j + k]);
	/* No trailing <Tab> at end of line. */
	if (j < (horizTotal - 1) * datumsize)
	  fprintf(outfile, "\t");
      }
      fprintf(outfile, "\n");
      lookedup = false;
    }
  }

  // All done. Close up shop.
  fclose(outfile);
  time_t stoptime = time(NULL);
  int elapsed = stoptime - starttime;
  fprintf(logfile, "%s\n", argv[0]);
  fprintf(logfile, "Number of markers: %i\n", markerCount);
  fprintf(logfile, "Mode: random\n");
  fprintf(logfile, "Elapsed time: %i seconds\n\n", elapsed);
  fclose(logfile);

  status = H5Tclose (datumtype);
  status = H5Sclose (memspace_id);
  status = H5Sclose (dataspace_id);
  status = H5Dclose (dataset_id);
  status = H5Fclose (file_id);
  if (status >= 0) return 0;
  else return 1;

}

