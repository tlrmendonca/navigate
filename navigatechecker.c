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


    // --------------
    // ---- HELP ----
    // --------------
    // Read the map from maps_file into memory for validation
    int map_lines = sol_header[0];
    int map_cols = sol_header[1];
    int map[1000][1000]; // Adjust max size as needed

    for (int i = 0; i < map_lines; i++) {
      for (int j = 0; j < map_cols; j++) {
        fscanf(maps_file, "%d", &map[i][j]); //how to deal with this compiling warning
      }
    }

    // --------------
    // ---- HELP ----
    // --------------

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

    // ERROR CODE: 15
    // When the task is #1 (header[2] > 0), a solution is given (final energy > 0),
    // but the final energy is below the required target (header[2])
    if (sol_header[2] > 0 && sol_header[7] > 0 && sol_header[7] < sol_header[2]) {
      print_error(15, problem_number); // Final energy does not reach the target
      fclose(maps_file); fclose(solmaps_file); fclose(check_file);
      return 0;
    }


    // ERROR CODE: 16
    // When task is #2 (maximum energy, header[2] == -2), and a solution is given (positive final energy),
    // the final energy in solution must match exactly the one in the check file
    if (sol_header[2] == -2 && sol_header[7] > 0 && sol_header[7] != check_header[7]) {
      print_error(16, problem_number);
      fclose(maps_file); fclose(solmaps_file); fclose(check_file);
      return 0;
    }


    // Begin move sequence validation only if a valid solution is present
    if (sol_header[7] > 0) { 

      int expected_steps = sol_header[5]; // Number of steps expected from header
      int previous_row = sol_header[3]; // starting line
      int previous_col = sol_header[4]; // starting column

      // Track visited cells using a 2D array
      int visited[1000][1000] = {{0}}; // adjust size if needed
      int map_lines = sol_header[0];
      int map_cols = sol_header[1];

      int current_energy = sol_header[6]; // Initial energy from header

      // Mark the starting cell as visited
      visited[previous_row - 1][previous_col - 1] = 1;

      // Attempt to read each move (row, column, energy)
      for (int step = 0; step < expected_steps; step++) {
        int row, col, energy;

        // ERROR CODE: 17
        // Failed to read a complete move (less than 3 integers found)
        if (fscanf(solmaps_file, "%d %d %d", &row, &col, &energy) != 3) {
          print_error(17, problem_number);
          fclose(maps_file); fclose(solmaps_file); fclose(check_file);
          return 0;
        }

        // ERROR CODE: 18
        // Move must be exactly one cell away in cardinal direction
        int dr = abs(row - previous_row);
        int dc = abs(col - previous_col);
        if (!((dr == 1 && dc == 0) || (dr == 0 && dc == 1))) {
          print_error(18, problem_number);
          fclose(maps_file); fclose(solmaps_file); fclose(check_file);
          return 0;
        }

        // ERROR CODE: 19
        // Out of bounds or already visited
        if (row < 1 || row > map_lines || col < 1 || col > map_cols || visited[row - 1][col - 1]) {
          print_error(19, problem_number);
          fclose(maps_file); fclose(solmaps_file); fclose(check_file);
          return 0;
        }

        visited[row - 1][col - 1] = 1; // Mark as visited

        // Update previous position
        previous_row = row;
        previous_col = col;


        // ERROR CODE: 20
        if (energy != map[row - 1][col - 1]) {
          print_error(20, problem_number);
          fclose(maps_file); fclose(solmaps_file); fclose(check_file);
          return 0;
        }

        // Accumulate energy
        current_energy += energy;

        // ERROR CODE: 21
        // Energy must remain strictly positive after every move
        if (current_energy <= 0) {
          print_error(21, problem_number);
          fclose(maps_file); fclose(solmaps_file); fclose(check_file);
          return 0;
        }


        // TODO: Error 22+ go here
      }

      // ERROR CODE: 22
      // Final accumulated energy must match the value reported in the solution header
      if (current_energy != sol_header[7]) {
        print_error(22, problem_number);
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

  // ERROR CODE: 23
  // After processing all problems, .solmaps must not have leftover data
  int extra;
  if (fscanf(solmaps_file, "%d", &extra) == 1) {
    print_error(23, 0); // No problem_number for this one
    fclose(maps_file); fclose(solmaps_file); fclose(check_file);
    return 0;
  }

  fclose(maps_file);
  fclose(solmaps_file);
  fclose(check_file);
  return 0;
}