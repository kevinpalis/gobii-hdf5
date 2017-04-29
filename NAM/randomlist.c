/* randomlist.c, DEM 13mar17 
   Output a list of random positive integers. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

  FILE *outfile;
  int i, x;

  if (argc < 2) {
    printf("Usage: %s <outputCount> <max>\n", argv[0]);
    printf("E.g. %s 100000 955000\n", argv[0]);
    printf("Output <outputCount> random integers between 0 and <max>.\n");
    return 0;
  }
  /* Read the arguments. */
  int outputCount = atoi(argv[1]);
  int max = atoi(argv[2]);

  for (i = 0; i < outputCount; ++i) {
    x = rand() % max;
    printf("%i\n", x);
  }
  return 0;
}
