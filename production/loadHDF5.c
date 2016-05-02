/* loadHDF5.c, DEM 1may16, from: loadSEED.c, DEM 6apr2016, from storeSEED.c DEM 19dec2015
   from example code h5_crtdat.c etc.  */

/* Load one dataset in Intermediate File format into the specified HDF5 file. 
   Tab-separated IUPAC codes, no header row or column.
*/


#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {

  /* HDF5 variables */
  hid_t       file_id, dataset_id, dataspace_id, memspace_id;  /* identifiers */
  hsize_t     dims[2], dimsmem[2];;
  herr_t      status;
  /* Hyperslab dimensions */
  hsize_t    offset[2], stride[2], count[2], blocksize[2];

  FILE *infile;
  char *token;
  int rownum, outndx;
  char *row;
  row = malloc(1000000);

  if (argc < 3) {
    printf("Usage: %s <input file> <output HDF5 file>\n", argv[0]);
    printf("Example: %s /home/matthews/BitBucket/production/testIFLinput.txt /shared_data/HDF5/Maize/SeeD_unimputed.h5\n", argv[0]);
    return 0;
  }

  char *infilename = argv[1];
  char *h5file = argv[2];
  char *h5dataset = "allelematrix";

  /* Create a new HDF5 file using default properties. */
  file_id = H5Fcreate(h5file, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  /* Open the file.  */
  file_id = H5Fopen(h5file, H5F_ACC_RDWR, H5P_DEFAULT);

  /* Read the first line of the input file to get the number of samples. */
  infile = fopen (infilename, "r");
  fgets (row, 1000000, infile);
  outndx = 0;
  token = strtok(row, "\t");  
  while ((token = strtok(NULL, "\t\n\r"))) 
    outndx++;
  int SampleCount = outndx;
  fclose(infile);
  printf("Samples: %i\n", SampleCount+1);


  /* Read the whole file through to get the number of markers.  (wc -l? )*/
  infile = fopen(infilename, "r");
  int markernum = 0;
  while (fgets (row, 1000000, infile)) 
    markernum++;
  int MarkerCount = markernum;
  fclose(infile);
  printf("Markers: %i\n", MarkerCount+1);


  /* Create the data space for the dataset. */
  dims[0] = MarkerCount;
  dims[1] = SampleCount; 
  dataspace_id = H5Screate_simple(2, dims, NULL);

  /* Create the dataset. Each element is type CHAR. */
  dataset_id = H5Dcreate2(file_id, h5dataset, H5T_NATIVE_CHAR, dataspace_id, 
                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  /* Create a memory buffer space. */
  dimsmem[0] = 1; 
  dimsmem[1] = SampleCount;
  memspace_id = H5Screate_simple(2, dimsmem, NULL);

  /* Create the hyperslab dimensions */
  offset[0] = 0; offset[1] = 0;
  stride[0] = 1; stride[1] = 1; 
  count[0] = 1; count[1] = 1;
  blocksize[0] = 1; blocksize[1] = SampleCount; 

  char dset_data[100000];
  rownum = 0;
  infile = fopen(infilename, "r");
  while (fgets (row, 1000000, infile)) {
    token = strtok(row, "\t");
    outndx = 0;
    dset_data[outndx] = token[0];
    while ((token = strtok(NULL, "\t\n\r"))) {
      /* Read the rest of the input line into dset_data[]. */
      ++outndx;
      dset_data[outndx] = token[0];
    }
    /* Adjust the hyperslab row. */
    offset[0] = rownum;
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offset, stride, count, blocksize);
    /* Write the row. */
    status = H5Dwrite(dataset_id, H5T_NATIVE_CHAR, memspace_id, dataspace_id, H5P_DEFAULT, dset_data);
    ++rownum;

    /* /\* For debugging. Echo to stdout. *\/ */
    /* int i; */
    /* for (i = 0; i < SampleCount; ++i) { */
    /* 	printf("%c", dset_data[i]); */
    /* } */
    /* printf("\n"); */
  }
  fclose(infile);
  free(row);

  /* End access to the dataset and release resources used by it. */
  status = H5Dclose(dataset_id);

  /* End access to the data space. */ 
  status = H5Sclose(dataspace_id);

  /* Close the file. */
  status = H5Fclose(file_id);

  if (status) return 0;
  else return 1;
}
