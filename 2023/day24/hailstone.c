#include <stdlib.h>
#include <string.h>
#include "hailstone.h"
#include "file.h"

int parse_hailstone(char *line, Hailstone *h) {
  int matched = sscanf(line, "%lld, %lld, %lld @ %d, %d, %d\n",
                       &h->px, &h->py, &h->pz, &h->vx, &h->vy, &h->vz);
  return matched == 6;
}

void print_hailstone(Hailstone h) {
  printf("{\nPx: %lld,\nPy: %lld,\nPz: %lld,\nVx: %d,\nVy: %d,\nVz: %d\n}\n",
         h.px, h.py, h.pz, h.vx, h.vy, h.vz);
}

void parse_all_hailstones(FILE *fp, Hailstone **hs, int *hs_capacity, int *hs_size, int *line_count) {
  char *line = malloc(sizeof(char) * (MAX_LINE_LENGTH + 1)); // 1 more for '\0'
  Hailstone h;

  read_line(fp, line);
  while (parse_hailstone(line, &h) == 1) {
    // Reallocate array of hailstones when capacity reached
    if (*hs_size >= *hs_capacity) {
      Hailstone *new_hs;
      int new_capacity = *hs_capacity + BATCH_SIZE;
      if ((new_hs = realloc(*hs, sizeof(Hailstone) * new_capacity)) != NULL) {
        *hs = new_hs;
        *hs_capacity = new_capacity;
      } else {
        perror("Could not realloc hs");
        break;
      }
    }

    // Push parsed hailstone
    (*hs)[*hs_size] = h;
    printf("Pushed hailstone %d:", *line_count);
    print_hailstone((*hs)[*hs_size]);
    *hs_size = *hs_size + 1;

    read_line(fp, line);
    *line_count = *line_count + 1;
  }

  free(line);
}
