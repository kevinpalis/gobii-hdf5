/* haplarray_rand.c, 25jul17, from haplarray_list.c, 8may17, from haplarray.c, 4mar16...
   Test performance of looking up alleles for an arbitrary set of markers 
   for an arbitrary set of samples, non-contiguous. */
/* 3may17, Modify for NAM c6-10 tests. */
/* 8may17, Read markers and samples from a file list. Variable datumsize. */
/* 25jul17, Create a new random list of markers and samples for each run. */

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define H5FILE      "/local/data/NAM_HM32/NAMc6-10.h5"
#define DATASETNAME "/allelematrix" 
#define RANK  2                           /* number of dimensions */

int main (int argc, char *argv[]) {

  /* HDF5 variables */
  hsize_t     filedims[RANK], dimsm[RANK];   
  hsize_t     start[RANK], stride[RANK], count[RANK], block[RANK];
  hid_t       file_id, dataset_id;        /* handles */
  hid_t       dataspace_id, memspace_id; 
  herr_t      status;                   

  FILE *outfile;
  FILE *logfile;
  int i, j, s, m;
  int row;
  time_t starttime, stoptime;
  int elapsed;

  if (argc < 3) {
    printf("Usage: %s <number of markers> <number of samples>\n", argv[0]);
    printf("E.g. %s 1000 1000\n", argv[0]);
    printf("Fetch alleles for the specified numbers of random markers and samples.\n");
    printf("Output is in /tmp/haplarray.out and ./haplarray.log.\n");
    return 0;
  }
  /* Read the arguments. */
  int markercount = atoi(argv[1]);
  int samplecount = atoi(argv[2]);

  outfile = fopen ("/tmp/haplarray.out", "w");

  /* Write a log of the results. */
  logfile = fopen ("haplarray.log", "a");
  fprintf(logfile, "%i markers, %i samples\n", markercount, samplecount);
  fprintf(logfile, "Mode: %s\n", "random");

  starttime = time(NULL);

  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (H5FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);
  dataspace_id = H5Dget_space (dataset_id);

  /* Find the dimensions of the HDF5 file dataset. */
  H5Sget_simple_extent_dims(dataspace_id, filedims, NULL);
  int MarkerTotal = filedims[0];
  int SampleTotal = filedims[1];

  /* Create the lists of random marker and sample numbers. */
  int markers[markercount];              
  for (m = 0; m < markercount; ++m) 
    markers[m] = rand() % MarkerTotal;
  int samples[samplecount];              
  for (s = 0; s < samplecount; ++s) 
    samples[s] = rand() % SampleTotal;

  /* Determine the datatype and the size of an individual element. */
  hid_t datumtype = H5Dget_type(dataset_id);
  int datumsize = H5Tget_size(datumtype);

  /* Create memory space with size of the subset, one row. */
  dimsm[0] = 1;
  dimsm[1] = SampleTotal;
  char *rdata = malloc(SampleTotal * datumsize);  /* buffer for read */
  memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
  /* Select subset from file dataspace. */
  start[1] = 0;
  stride[0] = 1; stride[1] = 1;
  count[0] = 1; count[1] = 1;
  block[0] = 1; block[1] = SampleTotal;


  /* Step through the array of marker positions one at a time, outputting the matrix row for each. */
  int marker;
  for (i = 0; i < markercount; ++i) {
    marker = markers[i];
    /* Select subset from file dataspace. */
    start[0] = marker;
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    /* Read the hyperslab. */
    status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
    /* Write the results to the output file, as a tab-delimited string for each marker. */
    for (s = 0; s < samplecount; ++s) {
      j = samples[s];
      fprintf(outfile, "%c\t", rdata[j]);
    }
    fprintf(outfile, "\n");
  }
  fclose(outfile);
  stoptime = time(NULL);
  elapsed = stoptime - starttime;
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
