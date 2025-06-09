#ifndef HAILSTONE_H
#define HAILSTONE_H
#define BATCH_SIZE 100

#include <stdio.h>

typedef struct {
  long long px;
  long long py;
  long long pz;
  int vx;
  int vy;
  int vz;
} Hailstone;

int parse_hailstone(char *line, Hailstone *h);
void print_hailstone(Hailstone h);
void parse_all_hailstones(FILE *fp, Hailstone **hs, int *hs_capacity, int *hs_size, int *line_count);

#endif // !HAILSTONE_H
