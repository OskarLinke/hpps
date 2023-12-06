#include "numlib.h"
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

void skipspaces(FILE *f) {
  while (1) {
    int c = fgetc(f);
    if (!isspace(c)) {
      if (c != EOF) {
        ungetc(c, f);
      }
      return;
    }
  }
}

int read_uint_ascii(FILE *f, uint32_t *out) {
  int read = 0;
  uint32_t num = 0;

  while (1) {
    int c = fgetc(f);

    if (c >= '0' && c <= '9') {
      num = num * 10 + (c - '0');
    } else {
      if (c != EOF) {
        ungetc(c, f);
      }
      if (read == 0) {
        if (c == EOF) {
          return EOF;
        } else {
          return 1;
        }
      } else {
        *out = num;
        return 0;
      }
    }
    read++;
  }
}

int read_double_ascii(FILE *f, double *out) {
  (void)f; (void)out;
  assert(0);
}

int read_uint_le(FILE *f, uint32_t *out) {

  int b0, b1, b2, b3;
  b0 = fgetc(f);

  if (b0 == EOF) {
    return EOF;
  }

  b1 = fgetc(f);
  b2 = fgetc(f);
  b3 = fgetc(f);

  if (b1 == EOF || b2 == EOF || b3 == EOF) {
    return 1;
  }

  *out =
    ((uint32_t)b0)
    | ((uint32_t)b1 << 8)
    | ((uint32_t)b2 << 16)
    | ((uint32_t)b3 << 24);
  return 0;
}

int read_uint_be(FILE *f, uint32_t *out) {
    //Similar to read_uint_le, but it adds the bytes to the 
  //destination file in the opposite order
  int b0, b1, b2, b3;
  b0 = fgetc(f);

  if (b0 == EOF) {
    return EOF;
  }

  b1 = fgetc(f);
  b2 = fgetc(f);
  b3 = fgetc(f);

  if (b1 == EOF || b2 == EOF || b3 == EOF) {
    return 1;
  }

  *out =
    ((uint32_t)b3)
    | ((uint32_t)b2 << 8)
    | ((uint32_t)b1 << 16)
    | ((uint32_t)b0 << 24);
  return 0;
}

int read_double_bin(FILE *f, double *out) {
  //Implemented similarly to read_uint_le, but I think
  //I've just made a longer integer, and the number isn't
  //able to make decimals, which is an issue
  int b0, b1, b2, b3, b4, b5, b6, b7;
  b0 = fgetc(f);

  if (b0 == EOF) {
    return EOF;
  }

  b1 = fgetc(f);
  b2 = fgetc(f);
  b3 = fgetc(f);
  b4 = fgetc(f); 
  b5 = fgetc(f); 
  b6 = fgetc(f); 
  b7 = fgetc(f); 

  if (b1 == EOF || b2 == EOF || b3 == EOF || b4 == EOF || b5 == EOF || b6 == EOF || b7 == EOF) {
    return 1;
  }

  *out =
    ((uint64_t)b0)
    | ((uint64_t)b1 << 8)
    | ((uint64_t)b2 << 16)
    | ((uint64_t)b3 << 24)
    |((uint64_t)b4 << 32)
    |((uint64_t)b5 << 40)
    |((uint64_t)b6 << 48)
    |((uint64_t)b7 << 56)
  return 0;
}

int write_uint_ascii(FILE *f, uint32_t x) {
  if (fprintf(f, "%u", x) < 0) {
    return 1;
  } else {
    return 0;
  }
}

int write_double_ascii(FILE *f, double x) {
  //This is extremely similar to write_uint_ascii, but we use 
  // %f instead of %u to indicate that we are using floating point
  //number instead of integer
  if (fprintf(f, "%f", x) < 0) {
    return 1;
  } else {
    return 0;
  }
}

int write_uint_le(FILE *f, uint32_t x) {
  fputc(x>>0,  f);
  fputc(x>>8,  f);
  fputc(x>>16, f);
  fputc(x>>24, f);
  return 0;
}

int write_uint_be(FILE *f, uint32_t x) {
  //similar to write_uint_le, but it adds the source bytes
  //to the destination in the opposite order
  fputc(x>>24,  f);
  fputc(x>>16,  f);
  fputc(x>>8, f);
  fputc(x>>0, f);
  return 0;
}

int write_double_bin(FILE *f, double x) {
  //I think this also just acts as if a double is
  //a long integer, and is not able to make decimals
  //We will see when we can test
  fputc(x>>0,  f);
  fputc(x>>8,  f);
  fputc(x>>16, f);
  fputc(x>>24, f);
  fputc(x>>32,  f);
  fputc(x>>40,  f);
  fputc(x>>48, f);
  fputc(x>>56, f);
  return 0;
}
