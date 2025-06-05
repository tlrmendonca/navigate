#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void solve_problem(FILE* i_file, FILE* o_file, int Lines, int Columns, int start_l, int start_c, int k, int l2, int c2) {
  // Initialize Map
  int **map = (int **)malloc(Lines * sizeof(int *));
  for (int i = 0; i < Lines; i++)
    map[i] = (int *)malloc(Columns * sizeof(int));

  // Fill Map
  for (int i = 0; i < Lines; i++) {
    for (int j = 0; j < Columns; j++) {
      fscanf(i_file, "%d", &map[i][j]); // TODO: Check efficiency of scanning entire lines at a time
    }
  }
  
  // Check if the input is valid
  if (start_l < 1 || start_l >= Lines + 1 || start_c < 1 || start_c >= Columns + 1) {
    fprintf(o_file, "%d %d %d %d %d\n", Lines, Columns, start_l, start_c, k);
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


    int curr_l = start_l;
    int curr_c = start_c;
  
    // Deslocation vertically until we get to l2
    while (curr_l != l2) {
      if (l2 < curr_l){
        curr_l--;           // Goes up one line
      } else {
        curr_l++;           // Goes down one line
      }
      fprintf(o_file, "(%d,%d)\n", curr_l, curr_c);
    }
    // Deslocation horizontaly until we get to c2
    while (curr_c != c2) {
      if (c2 < curr_c){
        curr_c--;           // Goes left one line
      } else {
        curr_c++;           // Goes right one line
      }
      fprintf(o_file, "(%d,%d)\n", curr_l, curr_c);
    }
  }

  // Free allocated memory
  for (int i = 0; i < Lines; i++)
    free(map[i]);
  free(map);

  return;
}

int main(int argc, char *argv[]) {

  // Check arguments
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  // Input File
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  // Output File
  char output_filename[strlen(argv[1]) + 4]; // .1maps (6) becomes .sol1maps (9) + 1 for `\0`
  strcpy(output_filename, argv[1]);
  char *dot = strrchr(output_filename, '.');
  strcpy(dot, ".sol1maps");
  FILE *output_file = fopen(output_filename, "w");

  while (!feof(file)) {  

    // Problem Header
    int Lines, Columns, start_l, start_c, k, l2, c2;
    if (fscanf(file, "%d", &Lines) != 1) break;     // Double check for end of file
    fscanf(file, "%d", &Columns);
    fscanf(file, "%d", &start_l);
    fscanf(file, "%d", &start_c);
    fscanf(file, "%d", &k);

    if (k == 0) {
      fscanf(file, "%d", &l2);
      fscanf(file, "%d", &c2);
    }

    // Debug: Print Header
    // printf("Lines: %d, Columns: %d, Start: (%d, %d), k: %d\n", Lines, Columns, start_l, start_c, k);

    // Solve Current Problem
    solve_problem(file, output_file, Lines, Columns, start_l, start_c, k, l2, c2);
    fprintf(output_file, "\n"); // Add a newline after each problem output
  }

  fclose(file);
  fclose(output_file);
  return 0;
}