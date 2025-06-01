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