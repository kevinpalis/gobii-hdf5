/* haplarray_rand_snp.c, 11apr2020, from: */
/* haplarray_rand.c, 25jul17, from haplarray_list.c, 8may17, from haplarray.c, 4mar16...
   Test performance of looking up alleles for an arbitrary set of markers 
   for an arbitrary set of samples, non-contiguous. */
/* 3may17, Modify for NAM c6-10 tests. */
/* 8may17, Read markers and samples from a file list. Variable datumsize. */
/* 25jul17, Create a new random list of markers and samples for each run. */
/* 11apr2020, Read markers and samples from a file list. */

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
  herr_t      status;                   

  FILE *outfile;
  FILE *logfile;
  int i, j, k, s, m;
  time_t starttime, stoptime;
  int elapsed;

  if (argc < 3) {
    printf("Usage: %s h5file markerlist samplelist\n", argv[0]);
    printf("E.g. %s ../data/generated_dataset_snp.h5 ../randomlists/SNPs_only/h5_markers.1000 ../randomlists/SNPs_only/h5_samples.1000\n", argv[0]);
    printf("Fetch alleles for the specified random markers and samples.\n");
    printf("Output is in ./haplarray.out and ./haplarray.log.\n");
    return 0;
  }
  /* Read the arguments. */
  char *H5FILE = argv[1];
  char *markerlist = argv[2];
  char *samplelist = argv[3];

  outfile = fopen ("./haplarray.out", "w");

  /* Write a log of the results. */
  logfile = fopen ("haplarray.log", "a");
  fprintf(logfile, "%s\n", argv[0]);
  fprintf(logfile, "Mode: random\n");

  starttime = time(NULL);

  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (H5FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);
  dataspace_id = H5Dget_space (dataset_id);

  /* Determine the datatype and the size of an individual element. */
  hid_t datumtype = H5Dget_type(dataset_id);
  int datumsize = H5Tget_size(datumtype);

  /* Find the dimensions of the HDF5 file dataset. */
  H5Sget_simple_extent_dims(dataspace_id, filedims, NULL);
  int MarkerTotal = filedims[0];
  int SampleTotal = filedims[1];

  /* Read in the lists of random marker and sample numbers. */

  // To dimension the input array, how many markers are in markerlist?
  FILE *markerfile = fopen(markerlist, "r");
  char *row = malloc(100);
  int markerCount = 0;
  while (fgets (row, 100, markerfile) != NULL) 
    markerCount++;
  fclose(markerfile);
  int *markers = malloc(markerCount * sizeof(int));

  /* Read in the list of queried marker positions into array markers[]. */
  markerfile = fopen (markerlist, "r");
  char *listitem = malloc(100);
  i = 0;
  while (fgets(listitem, 100, markerfile)) {
    markers[i] = atoi(listitem);
    if (markers[i] > MarkerTotal) {
      printf("Marker number %i out of range.\n", markers[i]);
      return 1;
    }
    i++;
  }
  fclose(markerfile);

  // To dimension the input array, how many samples are in samplelist?
  FILE *samplefile = fopen(samplelist, "r");
  int sampleCount = 0;
  while (fgets (row, 100, samplefile) != NULL)
    sampleCount++;
  fclose(samplefile);
  int *samples = malloc(sampleCount * sizeof(int));

  /* Read in the list of queried sample positions into array samples[]. */
  samplefile = fopen (samplelist, "r");
  listitem = malloc(100);
  i = 0;
  while (fgets(listitem, 100, samplefile)) {
    samples[i] = atoi(listitem);
    if (samples[i] > SampleTotal) {
      printf("Sample number %i out of range.\n", samples[i]);
      return 1;
    }
    i++;
  }
  fclose(samplefile);
  fprintf(logfile, "%i markers, %i samples\n", markerCount, sampleCount);
  

  /* Create memory space with size of the HDF5 subset, one row. */
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
  for (i = 0; i < markerCount; ++i) {
    /* Select subset from file dataspace. */
    start[0] = markers[i];
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    /* Read the hyperslab. */
    status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
    /* Write the results to the output file, as a tab-delimited string for each marker. */
    for (s = 0; s < sampleCount; s = s + datumsize) {
      for (k = 0; k < datumsize; k++)
	fprintf(outfile, "%c", rdata[samples[s]*datumsize + k]);
      fprintf(outfile, "\t");
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
