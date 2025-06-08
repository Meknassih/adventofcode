#ifndef SCHEMATIC_H
#define SCHEMATIC_H
#include <stdio.h>

#define MAX_NUMBERS_PER_LINE 128

void init_section(FILE *fp, char *l1, char *l2);
int next_section(FILE *fp, char *l0, char *l1, char *l2);
int find_next_number_in_line(char *line, int start_from, int* start, int* end);
void get_valid_numbers_in_line(char *l0, char* l1, char* l2, int* valid_numbers);
int has_neighboring_symbol(int start, int end, char *l0, char* l1, char* l2);
#endif
