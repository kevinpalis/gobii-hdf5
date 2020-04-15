/* fetchmarkerlist_indel_lkp_sf2_tp_batch.c, 31mar20, from: */
/* fetchmarkerlist_indel_lkp_sf2_tp.c, 10mar20, from: */
/* fetchmarkerlist_indel_lkp_sf2.c, 2mar20, from: */
/* fetchmarkerlist_indel_lkp_sf.c, 22feb20, from: */
/* fetchmarkerlist_indel_lkp.c, 21nov19, from: */
/* fetchmarkerlist.c, 13nov19, from: */
/* fetchmarkerlist.c, 8jun16, from fetchmarker.c, 7may16 from: fetchsample.c, 7may16
   Fetch a set of entire rows, alleles for all samples for the specified list of markers. */
/* 13jun16, Detect and handle different datatypes. */
/* 7dec16, Return a row of "N"s if marker number is -1. */
/* 7dec16, Take the list of markers from a file instead of command arguments. */
/* 4may17, Output in either orientation. */
/* 13nov19, Write execution times to a log. */
/* 21nov19, Handle insertions, specified in separate text file *.h5.inserts. 
            Output only markers-fast. */
/* 22feb20  Allow samples-fast too. */
/*  2mar20  Correct bug in samples-fast (returning _all_ markers instead of markers[i]). */
/* 10mar20  Transpose the output from samples-fast to markers-tall, samples-wide (Hapmap format). */
/* 31mar20  Batch into 500-sample batches. */


#include "hdf5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#define RANK  2                           /* number of dimensions */

int main (int argc, char *argv[]) {

  /* HDF5 variables */
  hsize_t     filedims[RANK], dimsm[RANK];   
  hsize_t     start[RANK], stride[RANK], count[RANK], block[RANK];
  hid_t       file_id, dataset_id;        /* handles */
  hid_t       dataspace_id, memspace_id; 
  hid_t       datumtype;
  herr_t      status;                   

  char *h5dataset;
  int datumsize, i, j, k, s, c, m, elapsed;
  bool lookedup = false;

  if (argc < 6) {
    printf("Usage: %s <orientation> <HDF5 file> <insertsfile> <queryfile> <output file>\n", argv[0]);
    printf("E.g. %s samples-fast ../data/generated_dataset_tetraploid_lkp.h5 ../data/generated_dataset_tetraploid_lkp.h5.inserts ../randomlists/SNPs_only/h5_markers.1000 fetchmarkerlist.out\n", argv[0]);
    printf("Fetch alleles for the markers listed in the queryfile, for all samples.\n");
    printf("The queryfile contains marker position numbers, separated by newline.\n");
    printf("The insertsfile contains the insertion sequences for indel markers.\n");
    return 0;
  }
  /* Read the arguments. */
  if (strcmp(argv[1], "samples-fast") == 0)
    h5dataset = "/allelematrix_samples-fast";
  else
    h5dataset = "/allelematrix";
  char *h5filename = argv[2];
  char *insertsfilename = argv[3];
  char *queryfilename = argv[4];
  char *outfilename = argv[5];

  FILE *outfile = fopen (outfilename, "w");

  /* Write a log of the results. */
  FILE *logfile = fopen ("markerstest.log", "a");

  time_t starttime = time(NULL);
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
  int vertTotal = filedims[0];
  int horizTotal = filedims[1];
  int markerTotal;
  if (strcmp(argv[1], "samples-fast") == 0) 
    markerTotal = horizTotal;
  else
    markerTotal = vertTotal;
  
  /* Determine the datatype and the size of an individual element. */
  datumtype = H5Dget_type(dataset_id);
  datumsize = H5Tget_size(datumtype);

  // To dimension the query array, how many markers are in queryfile?
  FILE *queryfile = fopen(queryfilename, "r");
  char *row = malloc(200);  // Used for insertsfile too.
  int markerCount = 0;
  while (fgets (row, 10, queryfile) != NULL)
    markerCount++;
  fclose(queryfile);
  int *markers = malloc(markerCount * sizeof(int));

  /* Read in the list of queried marker positions into array markers[]. */
  queryfile = fopen (queryfilename, "r");
  char *listitem = malloc(10);
  i = 0;
  while (fgets(listitem, 10, queryfile) != NULL) {
    markers[i] = atoi(listitem);
    if (markers[i] > markerTotal) {
      printf("Marker number %i out of range.\n", markers[i]);
      return 1;
    }
    i++;
  }
  fclose(queryfile);

  /* To dimension the inserts arrays, how many markers are in insertsfile? */
  FILE *insertsfile = fopen(insertsfilename, "r");
  int insertCount = 0;
  while (fgets(row, 200, insertsfile) != NULL) 
    insertCount++;
  fclose (insertsfile);

  /* Read the insertion sequences and their marker numbers into memory arrays. */
  insertsfile = fopen(insertsfilename, "r");
  int *markernums = malloc(insertCount * sizeof(int));
  char **seqs = malloc(insertCount * sizeof(char *));
  // seqs[] holds the array of insert sequences.
  for (i = 0; i < insertCount; i++) {
    seqs[i] = malloc(110);  // Max length of insertion sequence is 100 bp.
    if (seqs[i] == NULL) {
      printf("malloc seqs[i] failed, i = %i\n", i);
      return 1;
    }
  }
  i = 0;
  while (fgets(row, 200, insertsfile) != NULL) {
    // markernums[] holds the array of which marker numbers (positions in the matrix) have inserts.
    markernums[i] = atoi(strtok(row, "\t"));
    strcpy(seqs[i], strtok(NULL, "\t\n"));
    i++;
  }

  /* Read the HDF5 file. */

  if (strcmp(argv[1], "samples-fast") == 0) {    // "transposed" orientation
    int sampleTotal = vertTotal;
    /* HDF5 parameters for input file */
    /* Create memory space with size of subset. */
    dimsm[0] = 1;
    dimsm[1] = horizTotal; 
    memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
    /* Select subset from file dataspace. */
    start[1] = 0;
    stride[0] = 1; stride[1] = 1;
    count[0] = 1; count[1] = 1;
    block[0] = 1; block[1] = horizTotal;
    char *rdata = malloc(horizTotal * datumsize);  /* buffer for read */

    /* HDF5 setup for temporary, transposed file */
    hsize_t     tfiledims[RANK], tdimsmem[RANK];   
    hsize_t     tstart[RANK], tstride[RANK], tcount[RANK], tblock[RANK];
    hid_t       tfile_id, tdataset_id;        /* handles */
    hid_t       tdataspace_id, tmemspace_id; 
    /* Create a new HDF5 file using default properties. */
    tfile_id = H5Fcreate("temporary.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    tfile_id = H5Fopen("temporary.h5", H5F_ACC_RDWR, H5P_DEFAULT);
    /* Create the data space and dataset. */
    tfiledims[0] = markerCount;
    tfiledims[1] = sampleTotal;
    tdataspace_id = H5Screate_simple(2, tfiledims, NULL);
    tdataset_id = H5Dcreate2(tfile_id, "temp", datumtype, tdataspace_id,
                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    /* Read in 500-sample batches. */
    int batchsize = 500;
    /* Create a memory buffer space. */
    tdimsmem[0] = 1;
    tdimsmem[1] = batchsize;
    tmemspace_id = H5Screate_simple(2, tdimsmem, NULL);
    /* Specify the hyperslab, one row of length $batchsize. */
    tstart[0] = 0;
    tstride[0] = 1; tstride[1] = 1;
    tcount[0] = 1; tcount[1] = 1;
    tblock[0] = 1; tblock[1] = batchsize;

    /* Read the input file, one row (sample) at a time. */
    /* Transpose as we extract, writing to the temporary HDF5 file. */

    /* Declare the 2D array for reading a batch of the input file into. */
    char **batch_data = malloc(markerCount * datumsize * sizeof(char *));
    for (i = 0; i < markerCount * datumsize; i++)
      batch_data[i] = malloc(batchsize);

    /* Declare a row of the transposed array. */
    char markerdata[batchsize * datumsize];

    /* Read the input h5 file into the 2D array, batchwise. */
    int samp; // sample number
    int batchcounter = 0;
    for (samp = 0; samp < sampleTotal; samp++) {
      start[0] = samp;
      if (batchcounter == batchsize) {
	/* We've read in a batch worth. Write it out. */
	/* Read the hyperslab, one complete row (sample), into array rdata. */
	status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
	status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
	/* Copy the alleles for the _queried_ markers, markers[i], into the 2D array batch_data. */
	for (i = 0; i < markerCount; i++) 
	  for (k = 0; k < datumsize; k++) {
	    batch_data[i*datumsize + k][batchcounter-1] = rdata[markers[i]*datumsize + k];
	  }
	/* Transpose array batch_data, one marker at a time, and write to the temporary HDF5 file. */
	tstart[1] = (samp - batchsize);
	for (i = 0; i < markerCount; i++) {
	  for (j = 0; j < batchcounter; j++)
	    for (k = 0; k < datumsize; k++) 
	      markerdata[(j * datumsize) + k] = batch_data[(i * datumsize) + k][j];
	  tstart[0] = i;
	  /* Write one row of batchsize columns. */
	  status = H5Sselect_hyperslab(tdataspace_id, H5S_SELECT_SET, tstart, tstride, tcount, tblock);
	  status = H5Dwrite(tdataset_id, datumtype, tmemspace_id, tdataspace_id, H5P_DEFAULT, markerdata);
	}
	batchcounter = 0;
      }
      else {
	/* Read the hyperslab, one complete row (sample), into array rdata. */
	status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
	status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
	/* Copy the alleles for the _queried_ markers, markers[i], into the 2D array batch_data. */
	for (i = 0; i < markerCount; i++) 
	  for (k = 0; k < datumsize; k++)
	    batch_data[i*datumsize + k][batchcounter] = rdata[markers[i]*datumsize + k];
      }
      batchcounter++;
    }
    /* Now flush out the leftover batch. */
    /* Read the hyperslab, one complete row (sample), into array rdata. */
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
    status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
    /* Copy the alleles for the _queried_ markers, markers[i], into the 2D array batch_data. */
    for (i = 0; i < markerCount; i++) 
      for (k = 0; k < datumsize; k++) 
	batch_data[i*datumsize + k][batchcounter-1] = rdata[markers[i]*datumsize + k];
    tstart[1] = (samp - batchsize); //??
    /* Transpose array batch_data, one marker at a time, and write to the temporary HDF5 file. */
    for (i = 0; i < markerCount; i++) {
      for (j = 0; j < batchcounter; j++) 
	for (k = 0; k < datumsize; k++) 
	  markerdata[(j * datumsize) + k] = batch_data[(i * datumsize) + k][j];
      tstart[0] = i;
      /* Write one row of batchsize columns. */
      status = H5Sselect_hyperslab(tdataspace_id, H5S_SELECT_SET, tstart, tstride, tcount, tblock);
      status = H5Dwrite(tdataset_id, datumtype, tmemspace_id, tdataspace_id, H5P_DEFAULT, markerdata);
    }
    // Done writing the temporary transposed HDF5 file.
    status = H5Dclose(tdataset_id);
    status = H5Sclose(tmemspace_id);
    status = H5Fclose(tfile_id);

    time_t laptime = time(NULL);
    elapsed = laptime - starttime;
    fprintf(logfile, "Time = %i sec. Temporary .h5 file built. Dumping to output.\n", elapsed);

    /* Now dump the temporary file to outfile, translating "1" to the insert sequences. */
    status = H5Fopen("temporary.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
    tdataset_id = H5Dopen2 (tfile_id, "temp", H5P_DEFAULT);
    tdataspace_id = H5Dget_space (tdataset_id);
    /* Create a memory buffer space. */
    tdimsmem[0] = sampleTotal;
    tdimsmem[1] = 1;
    tmemspace_id = H5Screate_simple(2, tdimsmem, NULL);
    /* Specify the hyperslab, one row. */
    tstart[1] = 0;
    tstride[0] = 1; tstride[1] = 1;
    tcount[0] = 1; tcount[1] = 1;
    tblock[0] = 1; tblock[1] = sampleTotal;
    char *tdata = malloc(sampleTotal * datumsize);  
    /* Read one row (marker) at a time, looking up the insert if it's an indel. */
    for (m = 0; m < markerCount; m++) {
      tstart[0] = m;
      status = H5Sselect_hyperslab(tdataspace_id, H5S_SELECT_SET, tstart, tstride, tcount, tblock);
      status = H5Dread (tdataset_id, datumtype, tmemspace_id, tdataspace_id, H5P_DEFAULT, tdata);
      s = 0;  // which position in the array of insert sequences
      while (markernums[s] < markers[m])
	s++;
      for (i = 0; i < sampleTotal; i++) {
	for (k = 0; k < datumsize; k++)
	  if (tdata[i*datumsize + k] == '1') // If it's an insert, look up and output the sequence. 
	    fprintf(outfile, "%s", seqs[s]);
	  else
	    fprintf(outfile, "%c", tdata[i*datumsize + k]);
    	/* No trailing <Tab> at end of line. */
    	/* if (i < markerCount - 1) */  //bugged
    	  fprintf(outfile, "\t");
      }
      fprintf(outfile, "\n");
    }
  }

  else {     // Orientation is markers-fast ("normal").
    /* Create memory space with size of subset, one row. */
    dimsm[0] = 1;
    dimsm[1] = horizTotal; 
    memspace_id = H5Screate_simple (RANK, dimsm, NULL); 
    /* Select subset from file dataspace. */
    start[1] = 0;
    stride[0] = 1; stride[1] = 1;
    count[0] = 1; count[1] = 1;
    block[0] = 1; block[1] = horizTotal;
    char *rdata = malloc(horizTotal * datumsize);  /* buffer for read */
    /* Read and output the alleles for each requested marker. */
    for (i = 0; i < markerCount; i++) {
      start[0] = markers[i]; // markers[i] is the number of the current marker, its position in the allelematrix.
      status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, stride, count, block);
      /* Read the hyperslab. */
      status = H5Dread (dataset_id, datumtype, memspace_id, dataspace_id, H5P_DEFAULT, rdata);
      /* Write the results to the output file, as a tab-delimited string for each row. */
      for (j = 0; j < horizTotal * datumsize; j = j + datumsize) {
	for (k = 0; k < datumsize; k++) 
	  // If the allele is an insertion, look up the sequence.
	  if (rdata[j + k] == '1') {
	    if (lookedup == false) {  // first encounter with this marker on this row of input file
	      // Look it up in the inserts arrays and output it. 
	      s = 0;
	      while (markernums[s] < markers[i])
		s++;
	      lookedup = true;
	    }
	    fprintf(outfile, "%s", seqs[s]);
	  }
	  else
	    if (rdata[j + k] != '\0')  // Chop off the trailing <null> character.
	      fprintf(outfile, "%c", rdata[j + k]);
	/* No trailing <Tab> at end of line. */
	if (j < (horizTotal - 1) * datumsize)
	  fprintf(outfile, "\t");
      }
      fprintf(outfile, "\n");
      lookedup = false;
    }
  }

  // All done. Close up shop.
  fclose(outfile);
  time_t stoptime = time(NULL);
  elapsed = stoptime - starttime;
  fprintf(logfile, "%s\n", argv[0]);
  fprintf(logfile, "Number of markers: %i\n", markerCount);
  fprintf(logfile, "Mode: random\n");
  fprintf(logfile, "Elapsed time: %i seconds\n\n", elapsed);
  fclose(logfile);

  status = H5Tclose (datumtype);
  status = H5Sclose (memspace_id);
  status = H5Sclose (dataspace_id);
  status = H5Dclose (dataset_id);
  status = H5Fclose (file_id);
  if (status >= 0) return 0;
  else return 1;

}

