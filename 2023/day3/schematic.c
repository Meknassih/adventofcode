#include "schematic.h"
#include "file.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>

void init_section(FILE *fp, char* l0, char* l1, char* l2) {
  rewind(fp);
  strcpy(l0, "");
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

int find_next_star_in_line(char *line, int start_from) {
  for (int i = start_from; i < strlen(line); i++) { // last char won't be read because '\0'
    char c = line[i];
    if (c == '*')
    {
      return i;
    }
  }

  return -1;
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

int explore_number(char *line, int x) {
  int i = x - 1;
  // Assuming numbers can't have more than 6 digits (usually 3)
  char *left_part = malloc(sizeof(char) * 4), // 3 digits + \0
    *right_part = malloc(sizeof(char) * 4),
    c = line[i];
  strcpy(left_part, "");
  strcpy(right_part, "");

  // Left
  while (isdigit(c)) {
    // Doing an unshift with c into left_part
    char *temp_left = malloc((strlen(left_part) + 1) * sizeof(char));
    strcpy(temp_left, left_part);
    char temp_c[2] = {c, '\0'};
    strcpy(left_part, temp_c);
    strcat(left_part, temp_left);
    free(temp_left);
    c = line[--i];
  }

  //Right
  i = x;
  c = line[i];
  while (isdigit(c)) {
    char temp_c[2] = {c, '\0'};
    strcat(right_part, temp_c);
    c = line[++i];
  }

  // Assembling
  char *final = malloc((strlen(left_part) + strlen(right_part) + 1) * sizeof(char));
  strcpy(final, "");
  strcat(final, left_part);
  strcat(final, right_part);
  int n = -1;
  if (strlen(final) > 0) {
    n = atoi(final);
  }
  free(left_part);
  free(right_part);
  free(final);

  return n;
}

void find_numbers_adjacent_to_star(int star_x, char *l0, char* l1, char* l2, int* numbers) {
  int l0_prev_was_digit = 0, l1_prev_was_digit = 0, l2_prev_was_digit = 0, numbers_i = 0;

  for (int i = star_x-1; i <= star_x+1; i++) {
    // Safety
    if (i < 0) i = 0;
    else if (i > MAX_LINE_LENGTH) i = MAX_LINE_LENGTH;

    if (strlen(l0) > 1 && isdigit(l0[i])) { // l0 is "\n" on the first section
      if (!l0_prev_was_digit) {
        numbers[numbers_i] = explore_number(l0, i);
        numbers_i++;
        l0_prev_was_digit = 1;
      }
    } else {
      l0_prev_was_digit = 0;
    }

    if (isdigit(l1[i])) {
      if (!l1_prev_was_digit) {
        numbers[numbers_i] = explore_number(l1, i);
        numbers_i++;
        l1_prev_was_digit = 1;
      }
    } else {
      l1_prev_was_digit = 0;
    }

    if (strlen(l2) > 1 && isdigit(l2[i])) { // l2 is "\n" on the last section
      if (!l2_prev_was_digit) {
        numbers[numbers_i] = explore_number(l2, i);
        numbers_i++;
        l2_prev_was_digit = 1;
      }
    } else {
      l2_prev_was_digit = 0;
    }
  }
}

int find_next_gear(char *l0, char *l1, char *l2, int start_from, int *parts) {
  int star_x = start_from-1; // Offsetting first run because we add 1 below
  while ((star_x = find_next_star_in_line(l1, star_x+1)) > -1) {
    // Maximum numbers around a star should not exceed 6
    int adjacent_numbers[12];
    memset(adjacent_numbers, -1, sizeof(adjacent_numbers));
    find_numbers_adjacent_to_star(star_x, l0, l1, l2, adjacent_numbers);
    int count = 0;
    for (int i = 0; i < 12; i++) {
      if (adjacent_numbers[i] > -1) {
        count++;
      }
    }

    // A gear is exactly two numbers adjacent to a star
    if (count == 2) {
      parts[0] = adjacent_numbers[0];
      parts[1] = adjacent_numbers[1];
      return star_x;
    }
  }

  // No more stars in this line
  parts[0] = -1;
  parts[1] = -1;
  return -1;
}

// int **gears is an array of pairs of ints, each pair being a gear
int get_gears_in_line(char *l0, char *l1, char *l2, int **gears) {
  int parts[2], x = -1, gear_count = 0; // x = -1 because we add 1 at the start of each run

  while ((x = find_next_gear(l0, l1, l2, x+1, parts))) {
    if (x < 0) break; // No more gears in this line
    gears[gear_count] = malloc(sizeof(int) * 2);
    memcpy(gears[gear_count], parts, sizeof(int) * 2);
    gear_count++;
  }

  return gear_count;
}
