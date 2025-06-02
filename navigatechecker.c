#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * solve_problem(FILE* file, int Lines, int Columns, int start_l, int start_c, int k, int l2, int c2) {
  // Initialize Map
  int **map = (int **)malloc(Lines * sizeof(int *));
  for (int i = 0; i < Lines; i++)
    map[i] = (int *)malloc(Columns * sizeof(int));

  // Fill Map
  for (int i = 0; i < Lines; i++) {
    for (int j = 0; j < Columns; j++) {
      fscanf(file, "%d", &map[i][j]); // TODO: Check efficiency of scanning entire lines at a time
    }
  }

  // Some other logic...
  printf("Map (%d x %d):\n", Lines, Columns);
  for (int i = 0; i < Lines; i++) {
    for (int j = 0; j < Columns; j++) {
      printf("%d ", map[i][j]);
    }
    printf("\n");
  }

  // Task 1: Identify the largest positive energy value in the given map that is k or fewer steps away from the initial position.
  int max_energy = 0; // Store the biggest positive value found
  int found= 0;       // Flat to identify if we found a value bigger than 0

  for (int i = 0; i < Lines; i++) {
    for (int j = 0; j < Columns; j++) {
      //Calculate the Manhattan distance between (i,j) and (start_l,start_c)
      int dist = abs(i - start_l) + abs(j - start_c);

      if (dist <= k) {
        int valor = map[i][j];

        if (valor > 0) {
          if (!found){
            // First positive cell within the radius
            max_energy = valor;
            found = 1;
          } else if (valor > max_energy){ 
            // Update if higher value found
            max_energy = valor;
          }
        }
      }
    }
  }

  if (found) {
    printf("The biggest value of positive energy found within %d spaces is: %d\n", k, max_energy);
  } else {
    printf("Did not find any positive value within %d spaces\n", k);
  }
  
  // Task 2: add all positive energy values ​​of the cells within the map that are at a distance less than or equal to k from the initial position
  int sum_energy = 0;

  for (int i = 0; i < Lines; i++) {
    for (int j = 0; j < Columns; j++) {
      //Calculate the Manhattan distance between (i,j) and (start_l,start_c)
      int dist = abs(i - start_l) + abs(j - start_c);

      if (dist <= k) {
        int valor = map[i][j];
        if (valor > 0) {
          sum_energy += valor;
        }
      }
    }
  }

  printf("The total value form all the positive energies within %d spaces is %d\n",k, sum_energy);

  // Free allocated memory
  for (int i = 0; i < Lines; i++)
    free(map[i]);
  free(map);

  return "Solution not implemented yet";
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
    // printf("Lines: %d, Columns: %d, Start: (%d, %d), k: %d", Lines, Columns, start_l, start_c, k);

    // Solve Current Problem
    char *result = solve_problem(file, Lines, Columns, start_l, start_c, k, l2, c2);
    fprintf(output_file, "%s\n", result);
  }

  fclose(file);
  fclose(output_file);
  return 0;
}