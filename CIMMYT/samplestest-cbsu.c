/* samplestest.c, 26jan2015, from h5fetchsample.c, 25jan2015
   Fetch a set of entire columns from a two-dimensional HDF5 file. */

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

#define DIM0_SUB  1000000                 /* size of subset */

int main (int argc, char *argv[]) {

  /* HDF5 variables */
  hsize_t     dims[RANK], dimsm[RANK];   
  hsize_t     start[RANK], stride[RANK], count[RANK], block[RANK];
  hid_t       file_id, dataset_id;        /* handles */
  hid_t       dataspace_id, memspace_id; 
  herr_t      status;                   

  int dim1_sub;               /* number of samples to be read */
  FILE *outfile;
  FILE *logfile;
  int i, j;
  int column;
  time_t starttime, stoptime;
  int elapsed;

  if (argc < 3) {
    printf("%s", "Usage: samplestest <number of samples> <contiguous|random>\n");
    printf("%s", "Sample numbering starts at 1, not 0.\n");
    printf("Output is in samplestest.out and samplestest.log.\n");
    return 0;
  }
  /* Read the arguments. */
  int sampletotal = atoi(argv[1]);
  char *mode = argv[2];
  char results[DIM0_SUB][sampletotal]; /* indexed by [marker][sample] */
  int header[sampletotal];

  /* Write a log of the results. */
  logfile = fopen ("samplestest.log", "a");
  fprintf(logfile, "Number of samples: %i\n", sampletotal);
  fprintf(logfile, "Mode: %s\n", mode);

  starttime = time(NULL);
  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (H5FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);

  if (strcmp(mode, "random") == 0) {
    /* Create memory space with size of subset. Get file dataspace. */
    dim1_sub = 1;
    dimsm[0] = DIM0_SUB;
    dimsm[1] = dim1_sub;
    memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
    dataspace_id = H5Dget_space (dataset_id);
    char rdata[DIM0_SUB][dim1_sub];  /* buffer for read */
    for (i = 0; i < sampletotal; ++i) {
      column = rand() % 4845;
      header[i] = column;
      /* Select subset from file dataspace. */
      start[0] = 0; start[1] = column;
      stride[0] = 1; stride[1] = 1;
      count[0] = 1; count[1] = 1;
      block[0] = DIM0_SUB; block[1] = 1;
      status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
      /* Read the hyperslab. */
      status = H5Dread (dataset_id, H5T_NATIVE_CHAR, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
      /* Copy this column from rdata[] to results[][]. */
      for (j = 0; j < DIM0_SUB; ++j) {
	results[j][i] = rdata[j][0];
      }
    }
    /* Write the results to the output file, as a comma-delimited string for each marker. */
    outfile = fopen ("samplestest.out", "w");
    /* Header is the sample numbers. */
    for (i = 0; i < sampletotal; ++i) {
      fprintf(outfile, "%i,", header[i]);
    }
    fprintf(outfile, "\n");
    for (j = 0; j < DIM0_SUB; ++j) {
      for (i = 0; i < sampletotal; ++i) {
	fprintf(outfile, "%c,", results[j][i]);
      }
      fprintf(outfile, "\n");
    }
    fclose(outfile);
    status = H5Sclose (memspace_id);
    status = H5Sclose (dataspace_id);
  }

  else if (strcmp(mode, "contiguous") == 0) {
    /* First column of the region */
    column = rand() % 1000;
    int firstsample = column + 1;
    /* Create memory space with size of subset. Get file dataspace. */
    dim1_sub = sampletotal;
    dimsm[0] = DIM0_SUB;
    dimsm[1] = dim1_sub;
    memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
    dataspace_id = H5Dget_space (dataset_id);
    char rdata[DIM0_SUB][dim1_sub];  /* buffer for read */
    /* Select subset from file dataspace. */
    start[0] = 0; start[1] = column;
    stride[0] = 1; stride[1] = 1;
    count[0] = 1; count[1] = 1;
    block[0] = DIM0_SUB; block[1] = sampletotal;
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    /* Read the hyperslab. */
    status = H5Dread (dataset_id, H5T_NATIVE_CHAR, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
    /* Write the results to the output file, as a comma-delimited string for each marker. */
    outfile = fopen ("samplestest.out", "w");
    fprintf(outfile, "First sample: %i\n", firstsample);
    for (j = 0; j < DIM0_SUB; ++j) {
      for (i = 0; i < sampletotal; ++i) {
	fprintf(outfile, "%c,", rdata[j][i]);
      }
      fprintf(outfile, "\n");
    }
    fclose(outfile);
    status = H5Sclose (memspace_id);
    status = H5Sclose (dataspace_id);
  }
  else {
    printf("Unrecognized argument: %s\n", argv[2]);
  }

  stoptime = time(NULL);
  elapsed = stoptime - starttime;
  fprintf(logfile, "Elapsed time: %i seconds\n\n", elapsed);
  fclose(logfile);

  status = H5Dclose (dataset_id);
  status = H5Fclose (file_id);
}
