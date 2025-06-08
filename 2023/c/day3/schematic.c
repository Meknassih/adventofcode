#include "schematic.h"
#include "file.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>

void init_section(FILE *fp, char* l1, char* l2) {
  read_line(fp, l1);
  read_line(fp, l2);
}

int next_section(FILE *fp, char* l0, char *l1, char *l2) {
  strcpy(l0, l1);
  strcpy(l1, l2);
  return read_line(fp, l2);
}


int is_symbol(char c) {
  return (!isdigit(c) && c != '.' && c != '\n');
}

int has_neighboring_symbol(int start, int end, char *l0, char* l1, char* l2) {
  /*
  l0> ..@@@@..
  l1> ..@12@..
  l2> ..@@@@..
  */
  for (int i = start - 1; i <= end; i++) {
    // Safety
    if (i<0)
      i = 0;
    else if (i>MAX_LINE_LENGTH)
      i = MAX_LINE_LENGTH;

    if (
      (strlen(l0) > 1 && is_symbol(l0[i])) || // l0 is "\n" on the first section
      is_symbol(l1[i]) ||
      (strlen(l2) > 1 && is_symbol(l2[i])) // l2 is "\n" on the last section
    )
    {
      return 1;
    }
  }

  return 0;
}

int find_next_number_in_line(char *line, int start_from, int* start, int* end) {
  int building = 0;
  char *built_number = malloc(8);
  strcpy(built_number, "");

  for (int i = start_from; i < strlen(line); i++) { // last char won't be read because '\0'
    char c = line[i];
    if (isdigit(c))
    {
      if (!building) {
        *start = i;
        building = 1;
      }
      if (strlen(built_number) + 2 > malloc_usable_size(built_number)) {
        printf("Trying to build number greater than %lu", malloc_usable_size(built_number));
        return -1;
      }
      char temp[2] = {c, '\0'};
      strcat(built_number, temp);
    } else if (building) {
      // Not a digit AND we were building a number = finished reading number
      *end = i;
      break;
    }
  }

  int n = -1; // Valid numbers cannot be negative
  if (strlen(built_number) > 0)
  {
    n = atoi(built_number);
  }
  free(built_number);
  return n;
}

void get_valid_numbers_in_line(char *l0, char* l1, char* l2, int* valid_numbers) {
    int n, valid_i = 0;
    int start = 0, end = 0;
    while ((n = find_next_number_in_line(l1, end, &start, &end)) >= 0)
    {
      if (has_neighboring_symbol(start, end, l0, l1, l2)) {
        valid_numbers[valid_i] = n;
        valid_i++;
      }
    }
}
