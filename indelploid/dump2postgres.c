/* dump2postgres.c, 15dec19, from:
   dumpdataset.c, 9jun16, from fetchsample.c, 7may106, from: samplestest.c, 26jan2015
   Fetch an entire dataset, alleles for all markers for all samples, in standard sites-fast orientation. */
/* 14jun16: Read the datatype from the file, instead of requiring a command-line argument. */
/* 15dec19: Dump the transposed, samples-fast dataset. Add marker and sample names from VCF. 
            Output JSON loadfile for Postgres. */

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define RANK  2       /* number of dimensions */

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

  if (argc < 4) {
    printf("Usage: %s <HDF5 file> <VCF file> <Postgres file>\n", argv[0]);
    printf("E.g. %s ../data/generated_dataset_snp.h5 ../data/generated_dataset_snp.vcf ../data/dumpdataset.out\n", argv[0]);
    printf("Fetch alleles for all markers, all samples.  Samples-fast orientation.\n");
    printf("Output JSON loadfile for Postgres.\n");
    return 0;
  }
  /* Read the arguments. */
  char *h5filename = argv[1];
  char *vcffilename = argv[2];
  char *outfilename = argv[3];

  // Read in the sample names from the first row of the VCF file.
  FILE *vcffile = fopen(vcffilename, "r");
  char *row = malloc(1000000);
  row = fgets (row, 100000, vcffile);
  char *token = malloc(202);
  // Ignore the first nine fields.
  token = strtok(row, "\t\n");
  for (i=1; i<8; i++)
    token = strtok(NULL, "\t\n");
  // Remaining 3000 fields are all sample names.
  char **samplenames = malloc(3000 * sizeof(char *));
  for (i=0; i<3000; i++)
    samplenames[i] = malloc(30);
  int samplenum = 0;
  while ((token = strtok(NULL, "\t\n"))) {
    strcpy(samplenames[samplenum], token);
    samplenum++;
  }
  // Read in the marker names from the first two columns of the VCF file, #CHROM  POS.
  char **markernames = malloc(20000000 * sizeof(char *));
  for (i=0; i < 20000000; i++)
    markernames[i] = malloc(30);
  char *token2 = malloc(202);
  int mn = 0;
  while (fgets (row, 1000000, vcffile)) {
    token = strtok(row, "\t");
    /* token2 = strsep(&row, "\t"); */
    token2 = strtok(NULL, "\t");
    strcpy(markernames[mn], token);
    strcat(markernames[mn], "_");
    strcat(markernames[mn], token2);
    if (mn % 1000000 == 0)
      printf("mn = %i, name = %s\n", mn, markernames[mn]);
    mn++;
  }

  h5dataset = "/allelematrix_samples-fast";
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
  int SampleTotal = filedims[0];
  int MarkerTotal = filedims[1];

  /* Determine the datatype and the size of an individual element. */
  datumtype = H5Dget_type(dataset_id);
  datumsize = H5Tget_size(datumtype);

  /* Create memory space with size of the dataset. */
  dimsm[0] = 1;
  dimsm[1] = MarkerTotal; 
  memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
  char* rdata = malloc(MarkerTotal * datumsize);  /* buffer for read */
  /* Select subset from file dataspace. */
  start[1] = 0;
  stride[0] = 1; stride[1] = 1;
  count[0] = 1; count[1] = 1;
  block[0] = 1; block[1] = MarkerTotal;
  
  outfile = fopen (outfilename, "w");
  char *alleles = malloc(202);
  for (j = 0; j < SampleTotal; j++) {
printf("sample = %s\n", samplenames[j]);  
    start[0] = j;
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    /* Read the hyperslab. */
    status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
    /* Output the index and sample name. */
    fprintf(outfile, "%i\t%s\t{", j+100, samplenames[j]);
    /* Output the markername:allele pairs in JSON */
    for (i = 0; i < MarkerTotal; i++) {
      for (k = 0; k < datumsize; k++)
      	alleles[k] = rdata[i*datumsize + k];
      fprintf(outfile, "\"%s\":\"%s\",", markernames[i], alleles);
    }
    fprintf(outfile, "}\n");
  }
  fclose(outfile);

  status = H5Tclose (datumtype);
  status = H5Sclose (memspace_id);
  status = H5Sclose (dataspace_id);
  status = H5Dclose (dataset_id);
  status = H5Fclose (file_id);
  if (status >= 0) return 0;
  else return 1;
}
