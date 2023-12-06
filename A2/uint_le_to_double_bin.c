#include "numlib.h"
#include <assert.h>

int main(int argc, char* argv[]) {
  assert(argc == 1);
  while (1) {
    uint32_t x;
    int r = read_uint_be(stdin, &x);
    if (r == 0) {
      if (write_double_bin(stdout, (double)x) != 0) {
        fprintf(stderr, "%s: output error\n", argv[0]);
        return 1;
      }
    } else if (r == EOF) {
      return 0;
    } else {
      fprintf(stderr, "%s: input error\n", argv[0]);
      return 1;
    }
  }
}
