#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "navigatefunctions.h"

// --------------
// --- FILES ----
// --------------

// Check if the file has .maps extension
// Returns 1 if it has the extension, 0 otherwise
int is_maps_extension(const char *filename) {
  int len = strlen(filename);
  if (len > 5 && strcmp(filename + len - 5, ".maps") == 0) {
    return 1;
  }
  return 0;
}

// Transform the filename to a .solmaps extension
void get_solmaps_filename(const char *filename, char *solmaps_filename) {
  strcpy(solmaps_filename, filename);
  char *dot = strrchr(solmaps_filename, '.');
  strcpy(dot, ".solmaps");

  return;
}

// Transform the filename to a .check extension
void get_check_filename(const char *filename, char *check_filename) {
  strcpy(check_filename, filename);
  char *dot = strrchr(check_filename, '.');
  strcpy(dot, ".check");

  return;
}

// --------------
// ---- READ ----
// --------------

// Read a possibly broken header
// Returns 0 if failed, 1 if successful with 7 ints found, and 2 if successful with 8 ints found
int read_header(int header[8], int expected, FILE *file) {
  int count = 0;
  while (count < expected && fscanf(file, "%d", &header[count]) == 1) {
    count++;
  }

  if (count < expected) {
    return 0;
  }

  return 1;
}

// Read a known to be correct header
// Returns 0 if failed, 1 if successful with 7 ints found, and 2 if successful with 8 ints found
int read_correct_header(int header[8], FILE *file) {
  int count = 0;
  while (count < 7 && fscanf(file, "%d", &header[count]) == 1) {
    count++;
  }

  if (count < 7) {
    return 0;
  }

  // Check if there is an 8th integer
  if (is_problem_header_well_defined(header)) {
    if (fscanf(file, "%d", &header[7]) == 1)
      return 2; // Read it
    else return 0;
  }

  return 1;
}

// Confirm the header is a valid problem header
// NOTE: Input:
// [0] Lines [1] Columns [2] Minimal Energy [3] l [4] c [5] k [6] Initial Energy
// NOTE2: The "header" is of size 8, but the last element is not actually part of it
int is_problem_header_well_defined(int header[8]) {
  // 1. Lines and Columns must be positive
  if (header[0] <= 0 || header[1] <= 0) {
    return 0;
  }
  // 2. Minimal Energy must be positive or -2
  if (header[2] <= 0 && header[2] != -2) {
    return 0;
  }
  // 2. l and c are unrestricted (???)
  // 3. k must be in [0, LxC]
  if (header[5] < 0 || header[5] > header[0] * header[1]) {
    return 0;
  }
  // 4. Initial Energy must be positive
  if (header[6] <= 0) {
    return 0;
  }

  return 1; // Valid
}

// --------------
// --- ERRORS ---
// --------------

// Print a given error format
// NOTE: Remember to include a \n at the end of the format
void print_error(int error_id, int problem_number) {

  switch (error_id) {
    case 1:
      printf("Invalid arguments or unable to open input file (extension .maps).\nError code 1.\n");
      break;
    case 2:
      printf("Unable to open file with solutions (extension .solmaps).\nError code 2.\n");
      break;
    case 3:
      printf("Unable to open verification file (extension .check).\nError code 3.\n");
      break;
    case 4:
      printf("Unable to read header first line of solution.\nError code 4 in file problem %d.\n", problem_number);
      break;
    case 5:
      printf("Number of lines is incorrect.\nError code 5 in file problem %d.\n", problem_number);
      break;
    case 6:
      printf("Number of columns is incorrect.\nError code 6 in file problem %d.\n", problem_number);
      break;
    case 7:
      printf("Problem variant is incorrect.\nError code 7 in file problem %d.\n", problem_number);
      break;
    case 8:
      printf("Starting line is incorrect.\nError code 8 in file problem %d.\n", problem_number);
      break;
    case 9:
      printf("Starting column is incorrect.\nError code 9 in file problem %d.\n", problem_number);
      break;
    case 10:
      printf("Number of steps is incorrect.\nError code 10 in file problem %d.\n", problem_number);
      break;
    case 11:
      printf("Initial energy is incorrect.\nError code 11 in file problem %d.\n", problem_number);
      break;
    case 12:
      printf("Produces a solution for a problem with no solution.\nError code 12 in file problem %d.\n", problem_number);
      break;
    case 13:
      printf("Fails to produce a solution for a solvable problem.\nError code 13 in file problem %d.\n", problem_number);
      break;
    case 14:
      printf("Final energy presented is illegal (not positive nor -1).\nError code 14 in file problem %d.\n", problem_number);
      break;
    case 15:
      printf("Final energy does not reach target.\nError code 15 in file problem %d.\n", problem_number);
      break;
    case 16:
      printf("Final energy presented is not the maximum achievable.\nError code 16 in file problem %d.\n", problem_number);
      break;
    case 17:
      printf("Unable to read a move in solution file (.solmaps).\nError code 17 in file problem %d.\n", problem_number);
      break;
    case 18:
      printf("Move is not up/down/left/right.\nError code 18 in file problem %d.\n", problem_number);
      break;
    case 19:
      printf("Move is illegal: goes out of bounds or repeats a visited cell.\nError code 19 in file problem %d.\n", problem_number);
      break;
    case 20:
      printf("Prize/penalty incorrect for that cell.\nError code 20 in file problem %d.\n", problem_number);
      break;
    case 21:
      printf("Energy has reached zero or negative value along the path.\nError code 21 in file problem %d.\n", problem_number);
      break;
    case 22:
      printf("Final energy presented is inconsistent with sequence of prizes/penalties.\nError code 22 in file problem %d.\n", problem_number);
      break;
    case 23:
      printf("Excessive data on solution file.\nError code 23 at the end of file.\n");
      break;
    
    default:
      // Debug
      printf("Unknown error code %d in file problem %d.\n", error_id, problem_number);
      break;
  }

  return;
}