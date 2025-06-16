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
void get_solmaps_filename(const char *filename, char *solmaps_filename);

// Transform the filename to a .check extension
void get_check_filename(const char *filename, char *check_filename);

// --------------
// ---- READ ----
// --------------

// Read a header
// Returns 0 if failed, 1 if successful with 7 ints found, and 2 if successful with 8 ints found
int read_header(int header[8], int expected, FILE *file);

int read_correct_header(int header[8], FILE *file);

int is_problem_header_well_defined(int header[8]);

// --------------
// --- ERRORS ---
// --------------

// Print a given error format
// NOTE: Remember to include a \n at the end of the format
void print_error(int error_id, int problem_number);

#endif