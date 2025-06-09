#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "schematic.h"

int part2(FILE *fp, char *line0, char *line1, char *line2) {
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

    int **line_gears = malloc(sizeof(int *) * MAX_GEARS_PER_SECTION);
    int gear_count = get_gears_in_line(line0, line1, line2, line_gears);
    int line_sum = 0;
    for (int i = 0; i < gear_count; i++)
    {
      printf("[%d, %d], ", line_gears[i][0], line_gears[i][1]);
      line_sum += line_gears[i][0] * line_gears[i][1];
      free(line_gears[i]);
    }
    printf("\nline sum: %d\n", line_sum);
    printf("------\n");
    sum += line_sum;

    line_count++;
    was_eof = next_section(fp, line0, line1, line2);
    free(line_gears);
  }
  return sum;
}

int part1(FILE *fp, char *line0, char *line1, char *line2, int *valid_numbers) {
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
  return sum;
}

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
  init_section(fp, line0, line1, line2);
  printf("Part 1: %d\n", part1(fp, line0, line1, line2, valid_numbers));

  // Init with first two lines
  init_section(fp, line0, line1, line2);
  printf("Part 2: %d\n", part2(fp, line0, line1, line2));

  free(line0);
  free(line1);
  free(line2);
  free(valid_numbers);
  close_file(fp);
  return 0;
}

