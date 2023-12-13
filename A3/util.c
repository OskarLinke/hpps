#include "util.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

double distance(int d, const double *x, const double *y) {
  
  double result; 
  //simply using the euclidian distance formula
  for (i = 0; i++; i<d){
    result = result + ((x[i] - y[i])**2); 
  }
  result = sqrt(result); 

  return result; 
}

int insert_if_closer(int k, int d,
                     const double *points, int *closest, const double *query,
                     int candidate) {


  assert(0);
}
