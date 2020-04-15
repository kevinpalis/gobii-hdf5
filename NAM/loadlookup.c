/* loadlookup.c, 9apr18, from: */
/* loadNAMc6-10.c, from loadHDF5.c, 1jul2016, from: loadPhased.c, DEM 1may16, from: loadSEED.c, DEM 6apr2016, ...
   from example code h5_crtdat.c etc.  */

/* Load the lookup table of all allele values in the scoring dataset and their one-byte codes. 
   Codes are prefixed by I, D or V for Insertions, Deletions, and structural Variants.
   Each row contains the marker position in the allelematrix dataset followed by a list 
   of every allele of that marker and its code.
   27:IGC=1,IATA=2,D3=3
   53:ICTA=1,ITAG=2
   97:D5=1,D3=2
   ...
   31000000:IGCT=1
   D values are the number of nucleotides deleted. Maximum length of a row is 1000 bytes. 
   A code of 0 means no insertion or deletion, the reference allele.
   Results are stored in HDF5 dataset "lookup".
*/

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {

  /* HDF5 variables */
  hid_t       file_id, dataset_id, dataspace_id, memspace_id, rmemspace_id;  /* identifiers */
  hid_t       datumtype;
  hsize_t     dims[2], dimsmem[2];;
  herr_t      status;
  /* Hyperslab dimensions */
  hsize_t    offset[2], stride[2], count[2], blocksize[2];

  FILE *infile, *test;
  char *token;
  int rownum, i, j, k;
  char *row;

  if (argc < 3) {
    printf("Usage: %s <input file> <output HDF5 file>\n", argv[0]);
    printf("Example: %s lookup.txt ./lookup.h5\n", argv[0]);
    printf("Example: %s lookup.txt /local/data/NAM_H32/NAMc6-10.h5\n", argv[0]);
    return 0;
  }

  char *infilename = argv[1];
  char *h5file = argv[2];
  char *h5dataset = "lookup";

  /* Create a new HDF5 file using default properties. */
  /* Does the file already exist? */
  if (! (test = fopen(h5file, "r"))) 
    file_id = H5Fcreate(h5file, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  /* Open the file.  */
  /* It should not be still open from another process, see 
   https://support.hdfgroup.org/HDF5/doc/RM/RM_H5F.html#File-Open */
  file_id = H5Fopen(h5file, H5F_ACC_RDWR, H5P_DEFAULT);

  /* Read the whole file through to get the number of markers.  (wc -l?) */
  infile = fopen(infilename, "r");
  row = malloc(1000);
  int markernum = 0;
  while (fgets (row, 1000, infile) != NULL)
    markernum++;
  int MarkerCount = markernum;
  fclose(infile);
  printf("Markers: %i\n", MarkerCount);
  free(row);

  /* Create the data space for the dataset. */
  dims[0] = MarkerCount;
  dims[1] = 1;
  dataspace_id = H5Screate_simple(2, dims, NULL);

  /* Create the variable-length string datatype.
  /* datumtype = H5Tcreate(H5T_STRING,1000); */
  datumtype = H5Tcopy(H5T_C_S1);
  /* status = H5Tset_size(datumtype, H5T_VARIABLE); */
  status = H5Tset_size(datumtype, 1000);

  /* Create the dataset. Each element is type datumtype. */
  dataset_id = H5Dcreate2(file_id, h5dataset, datumtype, dataspace_id,
                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  /* Create a memory buffer space. */
  dimsmem[0] = 1;
  dimsmem[1] = 1;
  memspace_id = H5Screate_simple(2, dimsmem, NULL);

  /* Create the hyperslab dimensions */
  offset[0] = 0; offset[1] = 0;
  stride[0] = 1; stride[1] = 1;
  count[0] = 1; count[1] = 1;
  blocksize[0] = 1; blocksize[1] = 1;

  char *dset_data = malloc(1000);
  infile = fopen(infilename, "r");
  row = malloc(1000);
  rownum = 0;
  while (fgets (row, 1000, infile)) {
    /* Read the input line into dset_data[]. */
    i = 0;
    while (row[i] != '\n') {
      dset_data[i] = row[i];
      i++;
    }
    dset_data[i] = '\0';
    for (j = i + 1; j < 1000; j++)
      dset_data[j] = 0;
    /* Adjust the hyperslab row. */
    offset[0] = rownum;
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offset, stride, count, blocksize);
    /* Write the row. */
printf("%s\n", row);
    status = H5Dwrite(dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, dset_data);
    /* status = H5Dwrite(dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, row); */
    ++rownum;
  }
  fclose(infile);
  free(dset_data);
  free(row);

  /* End access to the dataset and release resources used by it. */
  status = H5Dclose(dataset_id);
  if (status < 0)
    return 2;
  /* End access to the data spaces. */ 
  status = H5Sclose(memspace_id);
  if (status < 0)
    return 3;
  status = H5Sclose(dataspace_id);
  if (status < 0)
    return 5;
  /* Close the file. */
  status = H5Fclose(file_id);
  if (status < 0)
    return 6;

  return 0;
}
