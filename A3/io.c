#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

double* read_points(FILE* f, int* n_out, int* d_out) {

  if (f == NULL) {
    return NULL;
  }

  if (fread(n_out, sizeof(int32_t), 1, f) != 1) {
    printf("Error reading n_out\n");
    return NULL;
  }

  if (fread(d_out, sizeof(int32_t), 1, f) != 1) {
    printf("Error reading d_out\n");
    return NULL;
  }

  double* result = malloc((*n_out) * (*d_out) * sizeof(double));

  if (fread(result, (*n_out) * (*d_out) * sizeof(double), 1, f) != 1) {
    printf("Error reading points\n");
    return NULL;
  }

  return result;

}


int* read_indexes(FILE *f, int *n_out, int *k_out) {
  
  if (f == NULL) {
    return NULL;
  }

  if (fread(n_out, sizeof(int32_t), 1, f) != 1) {
    printf("Error reading n_out\n");
    return NULL;
  }

  if (fread(k_out, sizeof(int32_t), 1, f) != 1) {
    printf("Error reading k_out\n");
    return NULL;
  }

  int* result = malloc((*n_out) * (*k_out) * sizeof(int));

  if (fread(result, (*n_out) * (*k_out) * sizeof(int), 1, f) != 1) {
    printf("Error reading indicies\n");
    return NULL;
  }

  return result;
}


int write_points(FILE *f, int32_t n, int32_t d, double* data) {
  
  if (f == NULL) {
    printf("Error with input file\n");
    return 1;
  }

  // Write number of points.
  if (fwrite(&n, sizeof(int32_t), 1, f) != 1) {
    return 1;
  }

  // Write number of values for each point (dimensionality).
  if (fwrite(&d, sizeof(int32_t), 1, f) != 1) {
    return 1;
  }
  
  
  if ((int)fwrite(data, n*d*sizeof(double), 1, f) != 1) {
    printf("Error writing data\n");
    return 1;
  }

  return 0;
}


int write_indexes(FILE *f, int32_t n, int32_t k, int *data) {

  if (f == NULL) {
    printf("Error with input file\n");
    return 1;
  }

  // Write number of points.
  if (fwrite(&n, sizeof(int32_t), 1, f) != 1) {
    perror("Error writing the n indicies");
    return 1;
  }

  // Write number of values for each point (dimensionality).
  if (fwrite(&k, sizeof(int32_t), 1, f) != 1) {
    perror("Error writing the k indicies");
    return 1;
  }
  
  // Write the indicies to the file
  if ((int)fwrite(data, n*k*sizeof(int), 1, f) != 1) {
    printf("Error writing data\n");
    return 1;
  }

  return 0;
}
