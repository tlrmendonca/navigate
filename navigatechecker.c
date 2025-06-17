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

  char c;
  while (fscanf(check_file, " %c", &c) == 1) {  
    ungetc(c, check_file);

    int check_header[8];
    int check_res = read_correct_header(check_header, check_file);
    // Note: check_header is never malformated

    // Solution File Problem Header
    int sol_header[8];
    int sol_res = read_header(sol_header, (check_res == 2) ? 8 : 7, solmaps_file);
    if (!sol_res) { // if 0 then error
      print_error(4, problem_number); // ERROR CODE: 4
      fclose(maps_file); fclose(solmaps_file); fclose(check_file);
      return 0;
    }

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
    // When the task is #1 (sol_header[2] > 0), a solution is given (check_header[7] > 0),
    // but the final energy is below the required target (header[2])
    if (sol_header[2] > 0 && check_header[7] > 0 && sol_header[7] < sol_header[2]) {
      print_error(15, problem_number); // Final energy does not reach the target
      fclose(maps_file); fclose(solmaps_file); fclose(check_file);
      return 0;
    }

    // ERROR CODE: 16
    // When task is #2 (sol_header[2] == -2), and a solution is given (check_header[7] > 0),
    // but the final energy in solution doesn't match exactly the one in the check file
    if (sol_header[2] == -2 && check_header[7] > 0 && sol_header[7] != check_header[7]) {
      print_error(16, problem_number);
      fclose(maps_file); fclose(solmaps_file); fclose(check_file);
      return 0;
    }

    // Begin move sequence validation only if a valid solution is present
    if (sol_header[7] > 0) {

      int error_found = 0; // Flag to track if any error is found (facilitate the frees)
      int expected_steps = sol_header[5]; // Number of steps expected from header
      
      // Track visited cells using a 2D array
      // expected_steps + 1 to account for the initial position
      int **visited = (int **)malloc((expected_steps + 1) * sizeof(int *));
      for (int i = 0; i <= expected_steps; i++) {
        visited[i] = (int *)malloc(3 * sizeof(int));
        visited[i][0] = -1; // Initialize to -1
        visited[i][1] = -1; // Initialize to -1
        visited[i][2] = -1; // Initialize to -1
      }
      
      // Mark the starting cell as visited
      visited[0][0] = sol_header[3];
      visited[0][1] = sol_header[4];
      visited[0][2] = sol_header[6];

      for (int step = 1; step <= expected_steps; step++) {
        // ERROR CODE: 17
        // Failed to read a complete move (less than 3 integers found)
        if (fscanf(solmaps_file, "%d %d %d", &visited[step][0], &visited[step][1], &visited[step][2]) != 3 ) {
          print_error(17, problem_number);
          fclose(maps_file); fclose(solmaps_file); fclose(check_file);
          error_found = 1;
          goto free_visited;
        }
      }
      
      // Helper variables
      int map_lines = sol_header[0];
      int map_cols = sol_header[1];
      int accumulated_energy = sol_header[6];

      // Finally read the map
      int maps_header[8];
      read_header(maps_header, 7, maps_file);
      
      // Initialize Map
      int **map = (int **)malloc(maps_header[0] * sizeof(int *));
      for (int i = 0; i < maps_header[0]; i++)
        map[i] = (int *)malloc(maps_header[1] * sizeof(int));

      // Fill Map
      for (int i = 0; i < maps_header[0]; i++) {
        for (int j = 0; j < maps_header[1]; j++) {
          if (fscanf(maps_file, "%d", &map[i][j]) != 1)
            return 0; // Something went wrong
        }
      }

      // Verify each move (row, column, energy)
      for (int step = 1; step <= expected_steps; step++) {
        int cur_row = visited[step][0];
        int cur_col = visited[step][1];
        int cell_energy = visited[step][2];

        // ERROR CODE: 18
        // Move must be exactly one cell away
        int moved_row = abs(cur_row - visited[step-1][0]);
        int moved_col = abs(cur_col - visited[step-1][1]);
        if ((moved_row + moved_col) != 1) {
          print_error(18, problem_number);
          fclose(maps_file); fclose(solmaps_file); fclose(check_file);
          error_found = 1;
          goto free_map;
        }

        // ERROR CODE: 19
        // Out of bounds or ...
        if (cur_row < 1 || cur_row > map_lines ||
            cur_col < 1 || cur_col > map_cols) {
          print_error(19, problem_number);
          fclose(maps_file); fclose(solmaps_file); fclose(check_file);
          error_found = 1;
          goto free_map;
        }
        // ... already visited
        for (int i = 0; i < step; i++) {
          if (visited[i][0] == cur_row && visited[i][1] == cur_col) {
            print_error(19, problem_number);
            fclose(maps_file); fclose(solmaps_file); fclose(check_file);
            error_found = 1;
            goto free_map;
          }
        }

        // ERROR CODE: 20
        // The energy on the step doesn't match the value on the map
        if (cell_energy != map[cur_row - 1][cur_col - 1]) {
          print_error(20, problem_number);
          fclose(maps_file); fclose(solmaps_file); fclose(check_file);
          error_found = 1;
          goto free_map;
        }

        // Accumulate energy
        accumulated_energy += cell_energy;

        // ERROR CODE: 21
        // Energy must remain strictly positive after every move
        if (accumulated_energy <= 0) {
          print_error(21, problem_number);
          fclose(maps_file); fclose(solmaps_file); fclose(check_file);
          error_found = 1;
          goto free_map;
        }
      }

      // ERROR CODE: 22
      // Final accumulated energy doesn't match the value in the solution header
      if (accumulated_energy != sol_header[7]) {
        print_error(22, problem_number);
        fclose(maps_file); fclose(solmaps_file); fclose(check_file);
        error_found = 1;
        goto free_map;
      }

      // Free blocks
      free_map:
      for (int i = 0; i < maps_header[0]; i++) {
        free(map[i]);
      }
      free(map);
    
      free_visited:
      for (int i = 0; i < expected_steps + 1; i++) {
        free(visited[i]);
      }
      free(visited);

      if (error_found) {
        return 0; // Exit if any error was found
      }
    }

    problem_number++;
  }

  // ERROR CODE: 23
  // After processing all problems, .solmaps must not have leftover data
  if (fscanf(solmaps_file, " %c", &c) == 1) {
    print_error(23, 0);
    fclose(maps_file); fclose(solmaps_file); fclose(check_file);
    return 0;
  }

  fclose(maps_file);
  fclose(solmaps_file);
  fclose(check_file);
  return 0;
}