/* h5fetch.c, DEM 29dec2015, from hdf5/examples/h5_subset.c 
   Fetch a single cell (subset) from a two-dimensional HDF5 file. */

#include "hdf5.h"
#include <stdlib.h>

#define FILE        "gobii.h5"
#define DATASETNAME "maize" 
#define RANK  2

#define DIM0_SUB  1                         /* subset dimensions */ 
#define DIM1_SUB  1 

#define DIM0     1000000                    /* size of dataset */       
#define DIM1     4845 

int main (int argc, char *argv[]) {

  /* HDF5 variables */
  hsize_t     dims[RANK], dimsm[RANK];   
  hid_t       file_id, dataset_id;        /* handles */
  hid_t       dataspace_id, memspace_id; 
  herr_t      status;                             
  hsize_t     count[2];              /* size of subset in the file */
  hsize_t     offset[2];             /* subset offset in the file */
  hsize_t     stride[2];
  hsize_t     block[2];
  char        rdata[DIM0_SUB][DIM1_SUB];          /* buffer for read */

  int i;

  if (argc < 3) {
    printf("%s", "Usage: h5fetch <Sample number> <Marker number>\n");
    return 0;
  }

  /* Specify size and shape of subset to read. */
  offset[0] = atoi(argv[2]);   /* Convert the command-line arguments to integers. */
  offset[1] = atoi(argv[1]);
  count[0]  = DIM0_SUB;  
  count[1]  = DIM1_SUB;
  stride[0] = 1;
  stride[1] = 1;
  block[0] = 1;
  block[1] = 1;

  /* Open the HDF5 file and dataset. */
  file_id = H5Fopen (FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, DATASETNAME, H5P_DEFAULT);

  /* Create memory space with size of subset. Get file dataspace
     and select subset from file dataspace. */
  dimsm[0] = DIM0_SUB;
  dimsm[1] = DIM1_SUB;
  memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
  dataspace_id = H5Dget_space (dataset_id);
  status = H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, offset,
				stride, count, block);

  /* Read the cell. */
  /* Use herr_t H5Sselect_elements( hid_t space_id, H5S_seloper_t op, size_t num_elements, const hsize_t *coord ) instead. */
  /* herr_t H5Dread( hid_t dataset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t xfer_plist_id, void * buf ) */
  status = H5Dread (dataset_id, H5T_NATIVE_CHAR, memspace_id, dataspace_id,
		    H5P_DEFAULT, rdata);

  /* printf ("\nDatum retrieved: "); */
  printf ("%c\n", rdata[0][0]);

  status = H5Sclose (memspace_id);
  status = H5Sclose (dataspace_id);
  status = H5Dclose (dataset_id);
  status = H5Fclose (file_id);
}
