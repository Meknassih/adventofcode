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
  char *line = malloc(MAX_LINE_LENGTH);
  strcpy(line, "");
  Hailstone *h = calloc(1, sizeof(Hailstone));

  read_line(fp, line);
  while (parse_hailstone(line, h) == 1) {
    // Reallocate array of hailstones when capacity reached
    if (*hs_size >= *hs_capacity) {
      Hailstone **new_hs;
      if ((new_hs = realloc(hs, *hs_capacity+BATCH_SIZE)) != NULL) {
        hs = new_hs;
        *hs_capacity += BATCH_SIZE;
      } else {
        perror("Could not realloc hs");
        break;
      }
    }

    // Push parsed hailstone
    hs[*hs_size] = malloc(sizeof(Hailstone));
    memcpy(hs[*hs_size], h, sizeof(Hailstone));
    printf("Pushed hailstone %d:", *line_count);
    print_hailstone(*hs[*hs_size]);
    (*hs_size)++;

    read_line(fp, line);
    (*line_count)++;
  }

  free(line);
  free(h);
}
