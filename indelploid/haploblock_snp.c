/* haploblock_snp.c, 11apr2020, from:
/* haploblock.c, 16feb2015, from samplestest.c, 26jan2015
   Fetch a contiguous block of rows and columns from a two-dimensional HDF5 file. */

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
  hsize_t     dimsm[RANK];   
  hsize_t     start[RANK], stride[RANK], count[RANK], block[RANK];
  hid_t       file_id, dataset_id;        /* handles */
  hid_t       dataspace_id, memspace_id; 
  herr_t      status;                   

  FILE *outfile;
  FILE *logfile;
  int i, j, k;
  int column, row;
  time_t starttime, stoptime;
  int elapsed;

  if (argc < 4) {
    printf("Usage: %s <HDF5 file> <number of markers> <number of samples>\n", argv[0]);
    printf("Example: %s ../data/generated_dataset_snp.h5 100 100\n", argv[0]);
    printf("Output is in ./haplotest.out and ./haplotest.log.\n");
    return 0;
  }
  /* Read the arguments. */
  char *H5FILE = argv[1];
  int markertotal = atoi(argv[2]);
  int sampletotal = atoi(argv[3]);

  /* Write a log of the results. */
  logfile = fopen ("haplotest.log", "a");
  fprintf(logfile, "%s\n", argv[0]);
  fprintf(logfile, "Fetching allele calls for %i markers and %i samples.\n", markertotal, sampletotal);
  fprintf(logfile, "Mode: contiguous\n");

  starttime = time(NULL);
  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (H5FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);

  /* Determine the datatype and the size of an individual element. */
  hid_t datumtype = H5Dget_type(dataset_id);
  int datumsize = H5Tget_size(datumtype);

  /* First row of the region. Leave room for the max size of the region. */
  int firstmarker = rand() % 18000000;
  /* First column of the region */
  int firstsample;
  if (sampletotal == 3000)
    firstsample = 0;
  else
    firstsample = rand() % 900;

  /* Create memory space with size of subset. Get file dataspace. */
  dimsm[0] = 1;
  /* dimsm[1] = sampletotal * datumsize; */
  dimsm[1] = sampletotal;
  memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
  dataspace_id = H5Dget_space (dataset_id);
  char rdata[sampletotal * datumsize];
  remove("./haplotest.out");
  outfile = fopen ("./haplotest.out", "a");
  for (j = 0; j < markertotal; ++j) {
    /* Select subset from file dataspace. */
    start[0] = firstmarker + j; start[1] = firstsample;
    stride[0] = 1; stride[1] = 1;
    count[0] = 1; count[1] = 1;
    block[0] = 1; block[1] = sampletotal;
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    /* Read the hyperslab, one row. */
    status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
    /* Write the results to the output file, as a tab-delimited string for each marker. */
    for (i = 0; i < sampletotal; i++) {
      for (k = 0; k < datumsize; k++)
	fprintf(outfile, "%c", rdata[i*datumsize + k]);
      fprintf(outfile, "\t");
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
