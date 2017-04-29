/* fetchsampleblock.c, 25apr107, from fetchsamplelist_test.c, 24apr2017, from fetchsamplelist.c, 8jun2016...
   Fetch a set of entire rows, alleles for all markers for a specified contiguous block of samples. */
/* 12jun16, Detect and handle different datatypes. */

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
  int datumsize, j, k;

  if (argc < 4) {
    printf("Usage: %s <HDF5 file> <firstsample> <samplecount> <output file>\n", argv[0]);
    printf("E.g. %s /local/data/NAM_HM32/NAMc6-10.h5 17 1000 /local/data/NAM_HM32/fetchsampleblock.out\n", argv[0]);
    printf("Fetch alleles for samplecount samples beginning with firstsample, for all markers.\n");
    return 0;
  }
  /* Read the arguments. */
  char *h5filename = argv[1];
  int firstsample = atoi(argv[2]);
  int samplecount = atoi(argv[3]);
  char *outfilename = argv[4];
  outfile = fopen (outfilename, "w");

  /* Write a log of the results. */
  FILE *logfile = fopen ("samplestest.log", "a");
  fprintf(logfile, "Number of samples: %i\n", samplecount);
  fprintf(logfile, "Mode: contiguous\n");

  time_t starttime = time(NULL);

  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (h5filename, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);
  dataspace_id = H5Dget_space (dataset_id);

  /* Find the dimensions of the HDF5 file dataset. */
  H5Sget_simple_extent_dims(dataspace_id, filedims, NULL);
  int SampleTotal = filedims[0];
  int MarkerTotal = filedims[1];

  /* Determine the datatype and the size of an individual element. */
  datumtype = H5Dget_type(dataset_id);
  datumsize = H5Tget_size(datumtype);

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

  int sample;
  for (sample = firstsample; sample < firstsample+samplecount; sample++) {
    if (sample >= SampleTotal) {
      printf("Sample number %i out of range.\n", sample);
      return 1;
    }
    if (sample < 0) {
      /* Sample "position" is -1, missing. Return a row of Ns. */
      for (j = 0; j < MarkerTotal * datumsize; j = j + datumsize) {
	for (k = 0; k < datumsize; k++)
	  fprintf(outfile, "N");
	if (j < (MarkerTotal - 1) * datumsize)
	/* No trailing <Tab> at end of line. */
	  fprintf(outfile, "\t");
      }
      fprintf(outfile, "\n");
    }
    else {
      start[0] = sample;
      status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
      /* Read the hyperslab. */
      status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
      /* Write the results to the output file, as a tab-delimited string for each sample. */
      for (j = 0; j < MarkerTotal * datumsize; j = j + datumsize) {
	for (k = 0; k < datumsize; k++) 
	  fprintf(outfile, "%c", rdata[j + k]);
	/* No trailing <Tab> at end of line. */
	if (j < (MarkerTotal - 1) * datumsize)
	  fprintf(outfile, "\t");
      }
      fprintf(outfile, "\n");
    }
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
