/* haploblock.c, 16feb2015, from samplestest.c, 26jan2015
   Fetch a contiguous block of rows and columns from a two-dimensional HDF5 file. */

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define H5FILE      "/home/matthews/gobii.h5"
#define DATASETNAME "maize" 
#define RANK  2                           /* number of dimensions */

#define DIM0     1000000                  /* size of dataset */       
#define DIM1     4845 

/* #define DIM0_SUB  1000000                 /\* size of subset *\/ */

int main (int argc, char *argv[]) {

  /* HDF5 variables */
  hsize_t     dimsm[RANK];   
  hsize_t     start[RANK], stride[RANK], count[RANK], block[RANK];
  hid_t       file_id, dataset_id;        /* handles */
  hid_t       dataspace_id, memspace_id; 
  herr_t      status;                   

  FILE *outfile;
  FILE *logfile;
  int i, j;
  int column, row;
  time_t starttime, stoptime;
  int elapsed;

  if (argc < 3) {
    printf("Usage: %s <number of markers> <number of samples>\n", argv[0]);
    printf("Output is in haplotest.out and haplotest.log.\n");
    return 0;
  }
  /* Read the arguments. */
  int markertotal = atoi(argv[1]);
  int sampletotal = atoi(argv[2]);

  /* Write a log of the results. */
  logfile = fopen ("haplotest.log", "a");
  fprintf(logfile, "Fetching allele calls for %i markers and %i samples.\n", markertotal, sampletotal);
  fprintf(logfile, "Mode: contiguous\n");

  starttime = time(NULL);
  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (H5FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);

  /* First row of the region */
  row = rand() % 100000;
  int firstmarker = row + 1;
  /* First column of the region */
  column = rand() % 1000;
  int firstsample = column + 1;

  /* Create memory space with size of subset. Get file dataspace. */
  dimsm[0] = markertotal;
  dimsm[1] = sampletotal;
  memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
  dataspace_id = H5Dget_space (dataset_id);
  char rdata[markertotal][sampletotal];  /* buffer for read */
  /* Select subset from file dataspace. */
  start[0] = firstmarker; start[1] = firstsample;
  stride[0] = 1; stride[1] = 1;
  count[0] = 1; count[1] = 1;
  block[0] = markertotal; block[1] = sampletotal;
  status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
  /* Read the hyperslab. */
  status = H5Dread (dataset_id, H5T_NATIVE_CHAR, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
  /* Write the results to the output file, as a comma-delimited string for each marker. */
  outfile = fopen ("haplotest.out", "w");
  fprintf(outfile, "First marker: %i; first sample: %i\n", firstmarker, firstsample);
  for (j = 0; j < markertotal; ++j) {
    for (i = 0; i < sampletotal; ++i) {
      fprintf(outfile, "%c,", rdata[j][i]);
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
