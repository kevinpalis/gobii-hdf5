/* HDF5info.c, 1may18, from:
/* loadHDF5_chunked.c, 17oct17, from: loadHDF5_1x5258.c, 13sep17, from: loadHDF5_chunked.c, 9aug2017 ... */

/* Print the basic info about an HDF5 file: datumsize, markers, samples.  */

#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  /* HDF5 variables */
  hid_t       file_id, dataset_id, dataspace_id;  /* identifiers */
  hid_t       datumtype;
  hsize_t     filedims[2];
  herr_t      status;

  /* int datumsize, rownum, outndx, i, j, k; */
  int datumsize;
  FILE *test;
  int markercount;  /* Number of markers that are in the file. */
  int samplecount;  /* Number of samples that are in the file. */

  if (argc < 2) {
    printf("Usage: %s <HDF5 file>\n", argv[0]);
    printf("Example: %s /share/NAM_HM32/NAMc6-10.h5\n", argv[0]);
    return 0;
  }

  char *h5file = argv[1];
  char *h5dataset = "allelematrix";

  /* Does the file exist? */
  if (! (test = fopen(h5file, "r"))) {
    printf("Can't find file %s\n", h5file);
    exit(0);
  }
  file_id = H5Fopen(h5file, H5F_ACC_RDONLY, H5P_DEFAULT);
  dataset_id = H5Dopen2 (file_id, h5dataset, H5P_DEFAULT);
  datumtype = H5Dget_type(dataset_id);
  datumsize = H5Tget_size(datumtype);
  dataspace_id = H5Dget_space (dataset_id);
  /* Find the dimensions of the HDF5 file dataset's dataspace. */
  H5Sget_simple_extent_dims(dataspace_id, filedims, NULL);
  markercount = filedims[0];
  samplecount = filedims[1];
  printf("Datasize: %i\n", datumsize);
  printf("Markers: %i\n", markercount);
  printf("Samples: %i\n", samplecount);

  /* End access to the data space. */
  status = H5Sclose(dataspace_id);
  /* End access to the dataset and release resources used by it. */
  status = H5Dclose(dataset_id);
  /* Close the file. */
  status = H5Fclose(file_id);
  if (status < 0)
    return 6;

  return 0;
}
