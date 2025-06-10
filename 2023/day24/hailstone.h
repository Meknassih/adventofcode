#ifndef HAILSTONE_H
#define HAILSTONE_H
#define BATCH_SIZE 100
#define MIN_COORD_ZONE 200000000000000
#define MAX_COORD_ZONE 400000000000000
// #define MIN_COORD_ZONE 7
// #define MAX_COORD_ZONE 27

#include <stdio.h>

typedef struct {
  long long px;
  long long py;
  long long pz;
  int vx;
  int vy;
  int vz;
} Hailstone;

typedef struct {
  int i;
  int j;
} Couple;

int parse_hailstone(char *line, Hailstone *h);
void print_hailstone(Hailstone h);
void parse_all_hailstones(FILE *fp, Hailstone **hs, int *hs_capacity, int *hs_size, int *line_count);

#endif // !HAILSTONE_H
