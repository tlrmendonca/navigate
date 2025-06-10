#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "navigatefunctions.h"

void solve_problem(FILE* i_file, FILE* o_file, int Lines, int Columns, int start_l, int start_c, int k, int l2, int c2) {
  // Initialize Map
  int **map = (int **)malloc(Lines * sizeof(int *));
  for (int i = 0; i < Lines; i++)
    map[i] = (int *)malloc(Columns * sizeof(int));

  // Fill Map
  for (int i = 0; i < Lines; i++) {
    for (int j = 0; j < Columns; j++) {
      if (fscanf(i_file, "%d", &map[i][j]) != 1) break;
    }
  }
  
  // Check if the input is valid
  if (Lines < 1 || Columns < 1 || start_l < 1 || start_l >= Lines + 1 || start_c < 1 || start_c >= Columns + 1) {
    if (k != 0) 
      fprintf(o_file, "%d %d %d %d %d\n", Lines, Columns, start_l, start_c, k);
    else
      fprintf(o_file, "%d %d %d %d %d %d %d\n", Lines, Columns, start_l, start_c, k, l2, c2);
    return;
  }

  // Dubug: Print the map
  // printf("Map (%d x %d):\n", Lines, Columns);
  // for (int i = 0; i < Lines; i++) {
  //   for (int j = 0; j < Columns; j++) {
  //     printf("%d ", map[i][j]);
  //   }
  //   printf("\n");
  // }
  
  // Task 1: Identify the largest positive energy value in the given map that is k or fewer steps away from the initial position.
  if (k < 0) { 
    int max_energy = 0;   // Store the biggest positive value found
    int coord_l = start_l - 1;
    int coord_c = start_c - 1;

    for (int i = 0; i < Lines; i++) {
      for (int j = 0; j < Columns; j++) {
        // Calculate the distance between (i,j) and (start_l,start_c)
        int dist = abs(i - coord_l) + abs(j - coord_c);

        if (dist <= abs(k) && dist > 0) {
          int value = map[i][j];

          if (value > max_energy) {
            max_energy = value;   // Update max_energy if a higher value is found
          }
        }
      }
    }

    fprintf(o_file, "%d %d %d %d %d %d\n", Lines, Columns, start_l, start_c, k, max_energy);

  } else if (k > 0) {
    // Task 2: Add all positive energy values ​​of the cells within the map that are at a distance less than or equal to k from the initial position
    int sum_energy = 0;
    int coord_l = start_l - 1;
    int coord_c = start_c - 1;

    for (int i = 0; i < Lines; i++) {
      for (int j = 0; j < Columns; j++) {
        //Calculate the distance between (i,j) and (start_l,start_c)
        int dist = abs(i - coord_l) + abs(j - coord_c);

        if (dist <= k && dist > 0) {
          int value = map[i][j];
          if (value > 0) {
            sum_energy += value;
          }
        }
      }
    }


    fprintf(o_file, "%d %d %d %d %d %d\n", Lines, Columns, start_l, start_c, k, sum_energy);

  } else if (k == 0) {
    //Task 3: produc e the path from (l1, c1) to (l2, c2), traveling first vertically and then horizontally
    fprintf(o_file, "%d %d %d %d %d %d %d\n", Lines, Columns, start_l, start_c, k, l2, c2);

    // Check if the destination is valid
    if (l2 < 1 || l2 > Lines || c2 < 1 || c2 > Columns) {
      return;
    }

    int curr_l = start_l;
    int curr_c = start_c;
  
    // Deslocation vertically until we get to l2
    while (curr_l != l2) {
      if (l2 < curr_l){
        curr_l--;           // Goes up one line
      } else {
        curr_l++;           // Goes down one line
      }
      fprintf(o_file, "%d %d %d\n", curr_l, curr_c, map[curr_l - 1][curr_c - 1]);
    }
    // Deslocation horizontaly until we get to c2
    while (curr_c != c2) {
      if (c2 < curr_c){
        curr_c--;           // Goes left one line
      } else {
        curr_c++;           // Goes right one line
      }
      fprintf(o_file, "%d %d %d\n", curr_l, curr_c, map[curr_l - 1][curr_c - 1]);
    }
  }

  // Free allocated memory
  for (int i = 0; i < Lines; i++)
    free(map[i]);
  free(map);

  return;
}

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
  char *solmaps_filename = get_solmaps_filename(argv[1]);
  FILE *solmaps_file = fopen(solmaps_filename, "r");
  
  if (solmaps_file != NULL) {
    print_error(2, 0);
    fclose(maps_file); fclose(solmaps_file);
    return 0;
  }
  
  // Check File
  // ERROR CODE: 3
  char *check_filename = get_check_filename(argv[1]);
  FILE *check_file = fopen(check_filename, "r");

  if (check_file != NULL) {
    print_error(3, 0);
    fclose(maps_file); fclose(solmaps_file); fclose(check_file);
    return 0;
  }


  // Output File
  // char output_filename[strlen(argv[1]) + 4]; // .1maps (6) becomes .sol1maps (9) + 1 for `\0`
  // strcpy(output_filename, argv[1]);
  // char *dot = strrchr(output_filename, '.');
  // strcpy(dot, ".sol1maps");
  // FILE *output_file = fopen(output_filename, "w");

  int problem_number = 1;

  while (!feof(maps_file)) {  

    int ERROR = 0; // Error flag

    // Solution File Problem Header
    int sol_header[7];
    if (!read_header(sol_header, solmaps_file)) {
      print_error(4, problem_number);
      fclose(maps_file); fclose(solmaps_file); fclose(check_file);
      return 0;
    }
    

    int sol_lines, sol_columns, sol_l1, sol_c1, sol_k, sol_l2, sol_c2;
    if (fscanf(maps_file, "%d", &sol_lines) != 1) ERROR = 1;
    if (fscanf(maps_file, "%d", &sol_columns) != 1) ERROR = 1;; 
    if (fscanf(maps_file, "%d", &sol_l1) != 1) ERROR = 1; 
    if (fscanf(maps_file, "%d", &sol_c1) != 1) ERROR = 1; 
    if (fscanf(maps_file, "%d", &sol_k) != 1) ERROR = 1; 

    if (sol_k == 0 && !ERROR) {
      if (fscanf(maps_file, "%d", &sol_l2) != 1) ERROR = 1; 
      if (fscanf(maps_file, "%d", &sol_c2) != 1) ERROR = 1; 
    }

    // ERROR CODE: 4
    if (ERROR) {
      print_error(4, problem_number);
      fclose(maps_file); fclose(solmaps_file); fclose(check_file);
      return 0;
    }

    // Debug: Print Header
    // printf("Lines: %d, Columns: %d, Start: (%d, %d), k: %d\n", Lines, Columns, start_l, start_c, k);

    // Solve Current Problem
    //solve_problem(maps_file, output_file, Lines, Columns, start_l, start_c, k, l2, c2);

    // Keep track of the problem number
    problem_number++;
  }

  fclose(maps_file);
  fclose(solmaps_file);
  fclose(check_file);
  return 0;
}