#ifndef FILE_H
#define FILE_H
#define MAX_LINE_LENGTH 512

#include <stdio.h>

int read_line(FILE *file, char *str);
FILE *open_file(char *filename);
int close_file(FILE *file);
#endif
