#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "navigatefunctions.h"

// Implement functions from navigatefunctions.h

char *get_solmaps_filename(const char *filename) {
  // TODO: Properly test
  char solmaps_filename[strlen(filename) + 4];
  strcpy(solmaps_filename, filename);
  char *dot = strrchr(solmaps_filename, '.');
  strcpy(dot, ".sol1maps");

  return solmaps_filename;
}