#include "file.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>

FILE *open_file(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (ferror(fp) != 0) {
    perror(strcat("Could not open file: ", filename));
    return NULL;
  }
  return fp;
}

int close_file(FILE *fp) {
  if (fclose(fp) != 0) {
    perror("Could not close file");
    return 1;
  }
  return 0;
}

int read_line(FILE *file, char *str) {
  if (file == NULL) {
    perror("Cannot read line from NULL file");
    return 1;
  }
  if (malloc_usable_size(str) < MAX_LINE_LENGTH) {
    perror("Allocated size of str is too small");
    return 1;
  }
  if (fgets(str, MAX_LINE_LENGTH, file) == NULL) {
    strcpy(str, "");
    return 1;
  }
  if (feof(file)) {
    perror("Reached end of file before reading line");
    strcpy(str, "");
    return 1;
  }
  if (ferror(file) != 0) {
    perror("Could not read line");
    strcpy(str, "");
    return 1;
  }
  return 0;
}
