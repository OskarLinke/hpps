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
  // Reads ASCII floats in input stream 
  // and updates double out value to ASCII input float.

  double result = 0.0;  // Initialize result
  int has_sign = 0;

  // Check for the presence of a sign character '-'
  int sign_char = fgetc(f);

  if (sign_char == '-') {
    has_sign = 1;
  } else {
    // Unget the character if it's not a '-'
    ungetc(sign_char, f);
  }

  // Read the integer part before the '.'
  uint32_t integer_part;
  if (read_uint_ascii(f, &integer_part) != 0) {
      printf("Error reading integer part.\n");
      return 1;
    }

  // Remove '.' from filestream
  if (fgetc(f) != '.') {
    printf("Expected decimal point, got unexpected character.");
    return 2;
  }

  // Read the integer part after the '.'
  uint32_t decimal_int;
  if (read_uint_ascii(f, &decimal_int) != 0) {
    printf("Error reading decimal part.\n");
    return 3;
  }

  // Put it all together
  //double true_decimal = 0.0;
  double decimal_part = (double)decimal_int;
  while (decimal_part >= 1.0) { 
    decimal_part = decimal_part / 10;
  }

  result += integer_part;
  result += decimal_part;

  // If there is a sign, negate the whole expression
  if (has_sign) {
    result = -result;
  }
  
  *out = result;

  return 0; // Success
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

/*
int read_double_bin(FILE *f, double *out) {
  //Implemented similarly to read_uint_le, but I think
  //I've just made a longer integer, and the number isn't
  //able to make decimals, which is an issue
  double b0, b1, b2, b3, b4, b5, b6, b7;
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
    ((double)b0)
    | ((double)b1 << 8)
    | ((double)b2 << 16)
    | ((double)b3 << 24)
    |((double)b4 << 32)
    |((double)b5 << 40)
    |((double)b6 << 48)
    |((double)b7 << 56);
  return 0;
}
*/

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
  fputc(x>>24, f);
  fputc(x>>16, f);
  fputc(x>>8, f);
  fputc(x>>0, f);
  return 0;
}

/*
int write_double_bin(FILE *f, double x) {
  //I think this also just acts as if a double is
  //a long integer, and is not able to make decimals
  //We will see when we can test
  fputc(x>>0,  f);
  fputc(x>>8,  f);
  fputc(x>>16, f);
  fputc(x>>24, f);
  fputc(x>>32, f);
  fputc(x>>40, f);
  fputc(x>>48, f);
  fputc(x>>56, f);
  return 0;
}
*/