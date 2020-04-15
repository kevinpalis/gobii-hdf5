/* fetchmarkerlist_rand.c, 7may17, from fetchmarkerlist_test.c, 26apr17, from fetchmarkerlist.c, 8jun16...
   Fetch a set of entire rows, alleles for all samples for the specified list of markers. */
/* 13jun16, Detect and handle different datatypes. */
/* 7dec16, Return a row of "N"s if marker number is -1. */
/* 7dec16, Take the list of marker positions from a file instead of command arguments. */
/* 26apr17, Add parameter to query only the first n markers from the list. */
/* 7may17, Generate the list of marker positions internally with rand(). */

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

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
  int datumsize, i, j, k;

  if (argc < 4) {
    printf("Usage: %s <HDF5 file> <markercount> <output file>\n", argv[0]);
    printf("E.g. %s /local/data/NAM_HM32/NAMc6-10.h5 1000 /local/data/NAM_HM32/fetchmarkerlist.out\n", argv[0]);
    printf("Fetch alleles for <markercount> random marker positions, for all samples.\n");
    return 0;
  }
  /* Read the arguments. */
  char *h5filename = argv[1];
  int markercount = atoi(argv[2]);
  char *outfilename = argv[3];
  outfile = fopen (outfilename, "w");

  /* Write a log of the results. */
  FILE *logfile = fopen ("markerstest.log", "a");
  fprintf(logfile, "Number of markers: %i\n", markercount);
  fprintf(logfile, "Mode: random\n");

  time_t starttime = time(NULL);

  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (h5filename, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);
  dataspace_id = H5Dget_space (dataset_id);

  /* Find the dimensions of the HDF5 file dataset. */
  H5Sget_simple_extent_dims(dataspace_id, filedims, NULL);
  int MarkerTotal = filedims[0];
  int SampleTotal = filedims[1];

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

  /* Pick a random marker position and output the matrix row for it. */
  int markernum = 1;
  int marker;
  for (i = 0; i < markercount; ++i) {
    marker = rand() % 31000000;
    if (marker >= MarkerTotal) {
      printf("Marker number %i out of range.\n", marker);
      return 1;
    }
    start[0] = marker;
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    /* Read the hyperslab. */
    status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
    /* Write the results to the output file, as a tab-delimited string for each marker. */
    for (j = 0; j < SampleTotal * datumsize; j = j + datumsize) {
      for (k = 0; k < datumsize; k++) 
	fprintf(outfile, "%c", rdata[j + k]);
      /* No trailing <Tab> at end of line. */
      if (j < (SampleTotal - 1) * datumsize)
	fprintf(outfile, "\t");
    }
    fprintf(outfile, "\n");
  }
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
