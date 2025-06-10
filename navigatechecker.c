#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "navigatefunctions.h"

int main(int argc, char *argv[]) {

  // Input File
  // ERROR CODE: 1
  if (argc != 2 || !is_maps_extension(argv[1])) {
    print_error(1, 0);
    return 0;
  }
  FILE *maps_file = fopen(argv[1], "r");
  if (maps_file == NULL) {
    print_error(1, 0);
    return 0;
  }

  // Solutions File
  // ERROR CODE: 2
  char solmaps_filename[strlen(argv[1]) + 4]; // .maps (5) becomes .solmaps (9) + 1 for null terminator
  get_solmaps_filename(argv[1], solmaps_filename);
  FILE *solmaps_file = fopen(solmaps_filename, "r");
  
  if (solmaps_file == NULL) {
    print_error(2, 0);
    fclose(maps_file);
    return 0;
  }
  
  // Check File
  // ERROR CODE: 3
  char check_filename[strlen(argv[1]) + 2]; // .maps (5) becomes .check (6) + 1 for null terminator
  get_check_filename(argv[1], check_filename);
  FILE *check_file = fopen(check_filename, "r");

  if (check_file == NULL) {
    print_error(3, 0);
    fclose(maps_file); fclose(solmaps_file);
    return 0;
  }

  // Keep track of the problem number
  int problem_number = 1;

  while (!feof(maps_file)) {  

    int ERROR = 0; // Error flag

    // Solution File Problem Header
    int sol_header[8];
    int sol_res = read_header(sol_header, solmaps_file);
    if (!sol_res) {
      print_error(4, problem_number); // ERROR CODE: 4
      fclose(maps_file); fclose(solmaps_file); fclose(check_file);
      return 0;
    }

    int check_header[8];
    int check_res = read_header(check_header, check_file);
    // Note: check_header is never malformated

    // Check solution header against check header
    for (int i = 0; i < 7; i++) {
      if (sol_header[i] != check_header[i]) {
        print_error(5 + i, problem_number); // ERROR CODE: 5, 6, 7, 8, 9, 10, 11
        fclose(maps_file); fclose(solmaps_file); fclose(check_file);
        return 0;
      }
    }

    // Check 8th header integer thouroughly
    // NOTE: res == 2 means 8 integers were read into header buffer
    if (check_res == 2) {
      if (sol_header[7] > 0 && check_header[7] == -1) {
        // ERROR CODE: 12
        print_error(12, problem_number);
        fclose(maps_file); fclose(solmaps_file); fclose(check_file);
        return 0;
      }
      else if (sol_header[7] == -1 && check_header[7] > 0) {
        // ERROR CODE: 13
        print_error(13, problem_number);
        fclose(maps_file); fclose(solmaps_file); fclose(check_file);
        return 0;
      }
      else if (sol_header[7] != -1 && sol_header[7] <= 0) {
        // ERROR CODE: 14
        print_error(14, problem_number);
        fclose(maps_file); fclose(solmaps_file); fclose(check_file);
        return 0;
      }
    }



    // Debug: Print Header
    // printf("Lines: %d, Columns: %d, Start: (%d, %d), k: %d\n", Lines, Columns, start_l, start_c, k);

    // Solve Current Problem
    //solve_problem(maps_file, output_file, Lines, Columns, start_l, start_c, k, l2, c2);

    problem_number++;
  }

  fclose(maps_file);
  fclose(solmaps_file);
  fclose(check_file);
  return 0;
}