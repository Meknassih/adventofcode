#include <stdlib.h>
#include "file.h"
#include "hailstone.h"

int main(void) {
  FILE *fp = open_file("hailstones.txt");
  Hailstone *hs = malloc(sizeof(Hailstone) * BATCH_SIZE);
  Couple *hs_pairs_intersect = malloc(sizeof(Couple) * BATCH_SIZE);
  int line_count = 0,
    hs_capacity = BATCH_SIZE,
    pairs_capacity = BATCH_SIZE,
    hs_size = 0,
    pairs_size = 0;

  parse_all_hailstones(fp, &hs, &hs_capacity, &hs_size, &line_count);

  // Refactor this into part1
  for (int i = 0; i < hs_size; i++) {
    for (int j = 0; j < hs_size; j++) {
      int skip_this_pair = 0;
      // Don't check hailstone against itself
      if (i == j) skip_this_pair = 1;
      // Don't check already intersecting hailstones
      for (int k = 0; k < pairs_size; k++) {
        if (
          (hs_pairs_intersect[k].i == i && hs_pairs_intersect[k].j == j) ||
          (hs_pairs_intersect[k].j == i && hs_pairs_intersect[k].i == j)
        ) {
          skip_this_pair = 1;
        }
      }
      if (skip_this_pair) continue;

      // Finding the intersection
      long double a0 = hs[i].vy * -1,
        b0 = hs[i].vx,
        c0 = hs[i].vy * hs[i].px - hs[i].vx * hs[i].py,
        a1 = hs[j].vy * -1,
        b1 = hs[j].vx,
        c1 = hs[j].vy * hs[j].px - hs[j].vx * hs[j].py,
        intersect_x, intersect_y;
      if (a0 * b1 - a1 * b0 == 0) {
        printf("Hailstones %d and %d paths are parallel\n", i, j);
      } else {
        intersect_x = (b0 * c1 - b1 * c0) / (a0 * b1 - a1 * b0);
        intersect_y = (a1 * c0 - a0 * c1) / (a0 * b1 - a1 * b0);
        // Checking if intersection is after origin of each vector
        // If v dot OP >= 0 then P is after the origin of v
        float dpi = (hs[i].vx * (intersect_x - hs[i].px)) +
          (hs[i].vy * (intersect_y - hs[i].py));
        float dpj = (hs[j].vx * (intersect_x - hs[j].px)) +
          (hs[j].vy * (intersect_y - hs[j].py));
        if (dpi >= 0 && dpj >= 0) {
          if (
            intersect_x >= (long double)MIN_COORD_ZONE &&
            intersect_x <= (long double)MAX_COORD_ZONE &&
            intersect_y >= (long double)MIN_COORD_ZONE &&
            intersect_y <= (long double)MAX_COORD_ZONE
          ) {
            printf("Hailstone %d intersects %d at (%Lf, %Lf) inside the zone\n",
                   i, j, intersect_x, intersect_y);
            // Save intersection pair
            if (pairs_size >= pairs_capacity) {
              Couple *new_hs_pairs = realloc(hs_pairs_intersect, sizeof(Couple) * (pairs_capacity+BATCH_SIZE));
              if (new_hs_pairs == NULL) {
                perror("Failed to realloc hs_pairs_intersect");
                break;
              }
              hs_pairs_intersect = new_hs_pairs;
              pairs_capacity += BATCH_SIZE;
            }
            hs_pairs_intersect[pairs_size].i = i;
            hs_pairs_intersect[pairs_size].j = j;
            pairs_size++;
          } else {
            printf("Hailstone %d intersects %d at (%Lf, %Lf) outside the zone\n",
                   i, j, intersect_x, intersect_y);
          }
        }
      }
    }
  }

  printf("There was %d intersections\n", pairs_size);

  free(hs);
  free(hs_pairs_intersect);
  close_file(fp);
  return 0;
}
