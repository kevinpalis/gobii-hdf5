/* loadNAM.c, DEM 4feb16, from: storeseed.c, DEM 19dec2015
   from example code h5_crtdat.c etc.  */

/* Load marker allele data from /shared_data/NAM_HM32/csv/c2.csv etc.
   into /home/matthews/gobii.h5 */
/* Sample contents:
chrom,pos,gdist,bp,B73(PI550473):250028951,B97(PI564682):250027958,CML103(Ames27081):250027883,CML228(Ames27088):250027960,...
2,S2_10056,0,10056,C,C,C,C,C,T,C,N,C,T,T,...
Dimensions: 5258 samples, 12694464 markers
9434936 markers for c2.  Loads in 24 minutes.
*/

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char *argv[]) {

  /* HDF5 variables */
  hid_t       file_id, dataset_id, dataspace_id, memspace_id;  /* identifiers */
  hsize_t     dims[2], dimsmem[2];;
  herr_t      status;
  /* Hyperslab dimensions */
  hsize_t    offset[2], stride[2], count[2], blocksize[2];

  FILE *infile;
  char *row;   
  char *token;
  int rownum, outndx;
  char *infilename;
  char *h5file;
  char *h5dsname;

  if (argc != 6) {
    printf("Usage: %s <input vcf file> <HDF5 file> <sample count> <marker count> <dataset name>\n", argv[0]);
    printf("Example: %s /shared_data/NAM_HM32/csv/c2.csv 5258 9434936 gobii.h5 /maizenam\n", argv[0]);
    return 0;
  }
  infilename = argv[1];
  int SampleCount = atoi(argv[2]);
  int MarkerCount = atoi(argv[3]);
  h5file = argv[4];
  h5dsname = argv[5];

  char dset_data[SampleCount];

  /* Create a new HDF5 file using default properties. */
  file_id = H5Fcreate(h5file, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  /* Create the data space for the dataset. 5000 samples x 1M markers*/
  dims[0] = MarkerCount; 
  dims[1] = SampleCount; 
  dataspace_id = H5Screate_simple(2, dims, NULL);

  /* Create the dataset. Each element is type CHAR. */
  dataset_id = H5Dcreate2(file_id, h5dsname, H5T_NATIVE_CHAR, dataspace_id, 
                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  /* Open the file.  */
  file_id = H5Fopen(h5file, H5F_ACC_RDWR, H5P_DEFAULT);

  /* Create a memory buffer space. */
  dimsmem[0] = 1; 
  dimsmem[1] = SampleCount;
  memspace_id = H5Screate_simple(2, dimsmem, NULL);

  /* Create the hyperslab dimensions */
  offset[0] = 0; offset[1] = 0;
  stride[0] = 1; stride[1] = 1; 
  count[0] = 1; count[1] = 1;
  blocksize[0] = 1; blocksize[1] = SampleCount; 

  infile = fopen (infilename, "r");

  row = (char *) malloc(1000000);
  rownum = 0;
  while (fgets (row, 1000000, infile)) {
    token = strtok(row, ",");
    /* Omit the first rows, headers. */
    if (strstr(token, "chrom") == NULL ) {
      outndx = 1;
      while ((token = strtok(NULL, ","))) {
	/* Omit the four leading columns before the data (CHROM, POS, ID, etc.). */
	if (outndx > 3) {
	  /* Read the rest of the input line into dset_data[]. */
	  dset_data[outndx - 4] = token[0];
	}
	++outndx;
      }
      /* Adjust the hyperslab row. */
      offset[0] = rownum;
      status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offset, stride, count, blocksize);
      /* Write to the dataset. */
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
  if (status >= 0) return 0;
  else return 1;
}
