/* storeseed.c, DEM 19dec2015
   from example code h5_crtdat.c etc.  */

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main() {

  /* HDF5 variables */
  hid_t       file_id, dataset_id, dataspace_id, memspace_id;  /* identifiers */
  hsize_t     dims[2], dimsmem[2];;
  herr_t      status;
  /* Hyperslab dimensions */
  hsize_t    offset[2], stride[2], count[2], blocksize[2];

  int SampleCount = 4845;
  FILE *infile;
  char *row;   
  char dset_data[5000];
  char *token;
  int rownum, outndx;

  /* Create a new HDF5 file using default properties. */
  file_id = H5Fcreate("gobii.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  /* Create the data space for the dataset. 5000 samples x 1M markers*/
  dims[0] = 1000000; 
  dims[1] = SampleCount; 
  dataspace_id = H5Screate_simple(2, dims, NULL);

  /* Create the dataset. Each element is type CHAR. */
  dataset_id = H5Dcreate2(file_id, "/maize", H5T_NATIVE_CHAR, dataspace_id, 
                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  /* Open the file.  */
  file_id = H5Fopen("gobii.h5", H5F_ACC_RDWR, H5P_DEFAULT);

  /* Create a memory buffer space. */
  dimsmem[0] = 1; 
  dimsmem[1] = SampleCount;
  memspace_id = H5Screate_simple(2, dimsmem, NULL);

  /* Create the hyperslab dimensions */
  offset[0] = 0; offset[1] = 0;
  stride[0] = 1; stride[1] = 1; 
  count[0] = 1; count[1] = 1;
  blocksize[0] = 1; blocksize[1] = SampleCount; 

  /* infile = fopen ("AllZea-Beagle-chr10-head.hmp.txt", "r"); */
  /* infile = fopen ("AllZeaGBSv2.7_SEED_Beagle4_chr1.hmp.txt", "r"); */
  infile = fopen ("AllZeaGBSv2.7_SEED_Beagle4_ALL.hmp.txt", "r");

  row = (char *) malloc(1000000);
  rownum = 0;
  while (fgets (row, 1000000, infile)) {
    token = strtok(row, "\t");
    /* Omit the first row, headers. */
    if (strstr(token, "rs#") == NULL ) {
      outndx = 1;
      while ((token = strtok(NULL, "\t"))) {
	/* Omit the eleven leading columns before the data (rs#, alleles, chrom etc.). */
	if (outndx > 10) {
	  /* Read the rest of the input line into dset_data[]. */
	  dset_data[outndx - 11] = token[0];
	}
	++outndx;
      }
      /* Adjust the hyperslab row. */
      offset[0] = rownum;
      status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offset, stride, count, blocksize);
      /* Write the dataset. */
      status = H5Dwrite(dataset_id, H5T_NATIVE_CHAR, memspace_id, dataspace_id, H5P_DEFAULT, dset_data);
      /* Echo to stdout. */
      /* for (i = 0; i < SampleCount; ++i) { */
      /* 	printf("%c", dset_data[i]); */
      /* } */
      /* printf("\n"); */
      ++rownum;
    }
  }

  free (row);
  fclose (infile);

  /* End access to the dataset and release resources used by it. */
  status = H5Dclose(dataset_id);

  /* End access to the data space. */ 
  status = H5Sclose(dataspace_id);

  /* Close the file. */
  status = H5Fclose(file_id);

  if (status) return 0;
  else return 1;
}
