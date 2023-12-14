#include "util.h"
#include "sort.h"
#include "io.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct sort_env {
  int d;
  const double* points;
  const double* query;
};

int cmp_indexes(const int* x, const int* y, struct sort_env* env) {
  int d = env-> d;
  const double* query = env -> query;
  const double* points = env -> points;

  double* point1 = &points[(*x) *d]; 
  double* point2 = &points[(*y) *d];

  if (distance(d, point1, query) < distance(d, point2, query)) { 
    return -1;

  } else if(distance(d, point1, query) == distance(d, point2, query)){ 
    return 0;

  } else  { 
    return 1;
  }
}


double distance(int d, const double *x, const double *y) {

  double result = 0.0; 
  //simply using the euclidian distance formula
  for (int i = 0; i < d; i++) {
    result += pow((x[i] - y[i]), 2); 
  }

  if ( result == 0 ) {
    return result;
  }
  result = sqrt(result); 

  return result; 
}

int insert_if_closer(int k, int d,
  const double *points, int *closest, 
  const double *query, int candidate) {
  
  if (closest[k-1] == -1) {
    for (int i = 0; i < k-1; i++) {
      if (closest[i] == -1) {
        closest[i] = candidate;
        return 0;
      }
    }
    closest[k-1] = candidate;
    struct sort_env env; env.d = d; env.points = points; env.query = query;
    hpps_quicksort(closest, k, sizeof(int), (int (*)(const void*, const void*, void*))cmp_indexes, &env);

    return 0;
  }

  double df_cand = distance(d, &points[d*candidate], query); // distance from candidate
  double df_farthest = distance(d, &points[d*closest[k-1]], query); // distance from farthest point

  if (df_cand < df_farthest) {
    closest[k-1] = candidate;
    struct sort_env env; env.d = d; env.points = points; env.query = query;
    hpps_quicksort(closest, k, sizeof(int), (int (*)(const void*, const void*, void*))cmp_indexes, &env);
    return 0;
  }
  return 0;
}
