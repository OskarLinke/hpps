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
  printf("We have entered cmp_indexes() \n");
  int d = env-> d;
  printf("d = %d ", d); 
  const double* query = env -> query;
  printf("Query point: " );
  for (int j = 0; j < d; j++) {
          printf("%f ", query[j]);
}
printf("\n"); 


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

  double result =0.0; 
  //simply using the euclidian distance formula
  for (int i = 0; i < d; i++) {
    result = result + pow((x[i] - y[i]), 2); 
  }
  result = sqrt(result); 

  return result; 
}

int insert_if_closer(int k, int d,
  const double *points, int *closest, const double *query,
  int candidate) {
  
  
  if (closest[k-1] == -1) {
    for (int i = 0; i < k-1; i++) {
      if (closest[i] == -1) {
        closest[i] = candidate;
        return 0;
      }
    }
    closest[k-1] = candidate;
    printf("final index in closest %d \n", closest[k-1]);
    struct sort_env env; env.d = d; env.points = points; env.query = query;
    printf("her til godt nok, cand: %d\n", candidate);
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

int main (int argc, char* argv[]) {
  assert(argc == 2);

  FILE* f = fopen(argv[1], "r");

  int n, d;
  double* all_points = read_points(f, &n, &d);

  for (int i = 0; i < n; i++) {
      printf("Point %d: ", i);
      for (int j = 0; j < d; j++) {
          printf("%f ", all_points[i * d + j]);
      }
      printf("\n");
  }
  double* query = &all_points[0];
  printf("n is: %d\n", n);
  int* closest = malloc(n*sizeof(int));
  
  assert(closest != NULL);
  for (int i = 0; i < n; i++) {
    closest[i] = -1;
  }
  
  assert(closest[n-1] == -1);

  for (int i = 0; i < n; i++) {
    insert_if_closer(n, d, all_points, closest, query, i);
  }


  for (int i = 0; i < n; i++) {
    printf("%d, ", closest[i]);
    //printf("%f", )
  }
  free(closest);


  return 0;
} 