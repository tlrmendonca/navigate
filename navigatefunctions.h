#ifndef NAVIGATEFUNCTIONS_H
#define NAVIGATEFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------
// --- FILES ----
// --------------

// Check if the file has .maps extension
// Returns 1 if it has the extension, 0 otherwise
int is_maps_extension(const char *filename);

// Transform the filename to a .solmaps extension
char *get_solmaps_filename(const char *filename);

// Transform the filename to a .check extension
char *get_check_filename(const char *filename);

// --------------
// ---- READ ----
// --------------

// Read a header
// Returns 1 if the header was read successfully, 0 otherwise
int read_header(int header[7], FILE *file);

// --------------
// --- ERRORS ---
// --------------

// Print a given error format
// NOTE: Remember to include a \n at the end of the format
void print_error(int error_id, int problem_number);

#endif