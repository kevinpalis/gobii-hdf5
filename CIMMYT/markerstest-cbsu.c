/* markersstest.c, 27jan2015, from samplestest.c, 26jan2015
   Fetch a set of entire rows (markers) from a two-dimensional HDF5 file. */

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
#define DIM1_SUB  4845                    /* size of subset */

int main (int argc, char *argv[]) {

  /* HDF5 variables */
  hsize_t     dims[RANK], dimsm[RANK];   
  hsize_t     start[RANK], stride[RANK], count[RANK], block[RANK];
  hid_t       file_id, dataset_id;        /* handles */
  hid_t       dataspace_id, memspace_id; 
  herr_t      status;                   

  int dim0_sub;               /* number of markers to be read */
  FILE *outfile;
  FILE *logfile;
  int i, j;
  int row;
  time_t starttime, stoptime;
  int elapsed;

  if (argc < 3) {
    printf("%s", "Usage: markerstest <number of markers> <contiguous|random>\n");
    printf("%s", "Marker numbering starts at 1, not 0.\n");
    printf("Output is in markerstest.out and markerstest.log.\n");
    return 0;
  }
  /* Read the arguments. */
  int markertotal = atoi(argv[1]);
  char *mode = argv[2];                /* contiguous or random */
  char results[markertotal][DIM1_SUB]; /* indexed by [marker][sample] */

  /* Write a log of the results. */
  logfile = fopen ("markerstest.log", "a");
  fprintf(logfile, "Number of markers: %i\n", markertotal);
  fprintf(logfile, "Mode: %s\n", mode);

  starttime = time(NULL);
  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (H5FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);

  if (strcmp(mode, "random") == 0) {
    /* Write the results to the output file, as a comma-delimited string for each marker. */
    outfile = fopen ("markerstest.out", "w");
    /* Create memory space with size of subset. Get file dataspace. */
    dim0_sub = 1;
    dimsm[0] = dim0_sub;
    dimsm[1] = DIM1_SUB;
    memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
    dataspace_id = H5Dget_space (dataset_id);
    char rdata[dim0_sub][DIM1_SUB];  /* buffer for read */
    for (i = 0; i < markertotal; ++i) {
      row = rand() % DIM0;
      /* Select subset from file dataspace. */
      start[0] = row; start[1] = 0;
      stride[0] = 1; stride[1] = 1;
      count[0] = 1; count[1] = 1;
      block[0] = 1; block[1] = DIM1_SUB;
      status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
      /* Read the hyperslab. */
      status = H5Dread (dataset_id, H5T_NATIVE_CHAR, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
      /* Output the row number. */
      fprintf(outfile, "%i ", row);
      /* Output the allele calls. */
      for (j = 0; j < DIM1_SUB; ++j) {
	fprintf(outfile, "%c,", rdata[0][j]);
      }
      fprintf(outfile, "\n");
    }
    fclose(outfile);
    status = H5Sclose (memspace_id);
    status = H5Sclose (dataspace_id);
  }

  else if (strcmp(mode, "contiguous") == 0) {
    /* First row of the region */
    row = rand() % 100000;
    /* Create memory space with size of subset. Get file dataspace. */
    dim0_sub = markertotal;
    dimsm[0] = dim0_sub;
    dimsm[1] = DIM1_SUB;
    memspace_id = H5Screate_simple (RANK, dimsm, NULL);
    dataspace_id = H5Dget_space (dataset_id);
    char rdata[dim0_sub][DIM1_SUB];  /* buffer for read */
    /* Select subset from file dataspace. */
    start[0] = row; start[1] = 0;
    stride[0] = 1; stride[1] = 1;
    count[0] = 1; count[1] = 1;
    block[0] = markertotal; block[1] = DIM1_SUB;
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    /* Read the hyperslab. */
    status = H5Dread (dataset_id, H5T_NATIVE_CHAR, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
    /* Write the results to the output file, as a comma-delimited string for each marker. */
    outfile = fopen ("markerstest.out", "w");
    int thisrow;
    for (i = 0; i < markertotal; ++i) {
      thisrow = row + i;
      fprintf(outfile, "%i ", thisrow);
      for (j = 0; j < DIM1_SUB; ++j) {
  	fprintf(outfile, "%c,", rdata[i][j]);
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
