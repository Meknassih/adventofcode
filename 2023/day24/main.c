#include <stdlib.h>
#include "file.h"
#include "hailstone.h"

int main(void) {
  FILE *fp = open_file("hailstones_test.txt");
  Hailstone **hs = malloc(sizeof(Hailstone*) * BATCH_SIZE);
  int line_count = 0, hs_capacity = BATCH_SIZE, hs_size = 0;

  parse_all_hailstones(fp, hs, &hs_capacity, &hs_size, &line_count);

  for (int i=0; i<hs_size; i++) {
    free(hs[i]);
  }
  free(hs);
  close_file(fp);
  return 0;
}
