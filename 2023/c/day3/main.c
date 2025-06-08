#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "schematic.h"

int main(void) {
  FILE *fp = open_file("engine_schematic.txt");
  char *line0 = malloc(MAX_LINE_LENGTH);
  char *line1 = malloc(MAX_LINE_LENGTH); // this is the line that is processed
  char *line2 = malloc(MAX_LINE_LENGTH);
  int *valid_numbers = malloc(MAX_NUMBERS_PER_LINE * sizeof(int));
  strcpy(line0, "\n");
  strcpy(line1, "\n");
  strcpy(line2, "\n");

  // Init with first two lines
  init_section(fp, line1, line2);

  int line_count = 0, sum = 0, was_eof = 0, iterations_with_eof = 0;
  while (iterations_with_eof < 1 && line_count < 1000)
  {
    printf("Line: %d\n", line_count);
    printf("L0: %s", line0);
    printf("L1: %s", line1);
    printf("L2: %s", line2);
    if (was_eof == 1) {
      iterations_with_eof++;
    }

    memset(valid_numbers, 0, MAX_NUMBERS_PER_LINE * sizeof(int));
    get_valid_numbers_in_line(line0, line1, line2, valid_numbers);
    int line_sum = 0;
    for (int i = 0; i < MAX_NUMBERS_PER_LINE; i++)
    {
      printf("%d, ", valid_numbers[i]);
      line_sum += valid_numbers[i];
    }
    printf("\nline sum: %d\n", line_sum);
    printf("------\n");
    sum += line_sum;
    
    line_count++;
    was_eof = next_section(fp, line0, line1, line2);
  }

  printf("%d\n", sum);

  free(line0);
  free(line1);
  free(line2);
  free(valid_numbers);
  close_file(fp);
  return 0;
}
