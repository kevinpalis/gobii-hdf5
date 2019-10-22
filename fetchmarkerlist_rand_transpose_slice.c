/* fetchmarkerlist_rand_transposed_slice.c, 23aug17, from: fetchmarkerlist_rand_transposed.c, 22aug17, from: fetchmarkerlist.c, 8jun16 */
/* Fetch a set of entire rows, alleles for all samples for a random list of markers. */
/* 13jun16, Detect and handle different datatypes. */
/* 7dec16, Take the list of markers from a file instead of command arguments. */
/* 4may17, Output in either orientation. */
/* 7may17, Generate the list of marker positions internally with rand(). */
/* 23aug17, Output the samples-fast query by extracting 1-marker vertical hyperslabs. */
/* 28sep17, For the full NAM dataset, 80m markers, increase the maximum random number. */

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

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
  char *h5dataset;
  int datumsize, i, j, k;

  if (argc < 5) {
    printf("Usage: %s <orientation> <HDF5 file> <markercount> <output file>\n", argv[0]);
    printf("E.g. %s markers-fast /shared_data/NAM_HM32/NAM_1000x1000.h5 1000 /tmp/fetchmarkerlist.out\n", argv[0]);
    printf("Fetch alleles for <markercount> random marker positions, for all samples.\n");
    printf("<orientation> is either 'samples-fast' or 'markers-fast'.\n");
    printf("The samples-fast query extracts vertical 1-marker slices from the matrix.\n");
    return 0;
  }
  /* Read the arguments. */
  if (strcmp(argv[1], "samples-fast") == 0)
    h5dataset = "/allelematrix_samples-fast";
  else
    h5dataset = "/allelematrix";
  char *h5filename = argv[2];
  int markercount = atoi(argv[3]);
  char *outfilename = argv[4];
  outfile = fopen (outfilename, "w");

  /* Write a log of the results. */
  FILE *logfile = fopen ("markerstest.log", "a");
  fprintf(logfile, "Number of markers: %i; ", markercount);
  if (strcmp(argv[1], "samples-fast") == 0)
      fprintf(logfile, "Mode: random. Orientation: samples-fast by marker slice\n");
  else
    fprintf(logfile, "Mode: random. Orientation: %s\n", argv[1]);
  fprintf(logfile, "HDF5 file: %s\n", argv[2]);

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
  
  /* Determine the datatype and the size of an individual element. */
  datumtype = H5Dget_type(dataset_id);
  datumsize = H5Tget_size(datumtype);

  /* Pick a list of random marker positions. */
  int *markers = malloc(markercount * sizeof(int));
  srand(starttime);
  for (i = 0; i < markercount; ++i) 
    markers[i] = rand() % 80000000;
  
  if (strcmp(argv[1], "samples-fast") == 0) {
    // "transposed" orientation
    /* Create memory space with size of subset. */
    dimsm[0] = vertTotal;
    dimsm[1] = 1; 
    memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
    /* Select subset from file dataspace. */
    start[0] = 0;
    stride[0] = 1; stride[1] = 1;
    count[0] = 1; count[1] = 1;
    block[0] = vertTotal; block[1] = 1;
    char *rdata = malloc(vertTotal * datumsize);  /* buffer for read */
    int m;
    for (m = 0; m < markercount; m++) {
      start[1] = markers[m]; 
      status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
      /* Read the hyperslab. */
      status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
      for (i = 0; i < vertTotal; i++) {
	for (k = 0; k < datumsize; k++) 
	  fprintf(outfile, "%c", rdata[i * datumsize + k]);
	/* No trailing <Tab> at end of line. */
	if (i < (vertTotal - 1))
	  fprintf(outfile, "\t");
      }
      fprintf(outfile, "\n");
    }
  }

  else {
    // Orientation is markers-fast ("normal").

    /* Create memory space with size of subset. */
    dimsm[0] = 1;
    dimsm[1] = horizTotal; 
    memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
    /* Select subset from file dataspace. */
    start[1] = 0;
    stride[0] = 1; stride[1] = 1;
    count[0] = 1; count[1] = 1;
    block[0] = 1; block[1] = horizTotal;
    char rdata[horizTotal * datumsize];  /* buffer for read */

    for (i = 0; i < markercount; i++) {
      /* It's a valid marker position. Read and output the alleles. */
      start[0] = markers[i];
      status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
      /* Read the hyperslab. */
      status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
      /* Write the results to the output file, as a tab-delimited string for each row. */
      for (j = 0; j < horizTotal * datumsize; j = j + datumsize) {
	for (k = 0; k < datumsize; k++) 
	  fprintf(outfile, "%c", rdata[j + k]);
	/* No trailing <Tab> at end of line. */
	if (j < (horizTotal - 1) * datumsize)
	  fprintf(outfile, "\t");
      }
      fprintf(outfile, "\n");
    }
  }
  fclose(outfile);
  time_t stoptime = time(NULL);
  int elapsed = stoptime - starttime;
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
