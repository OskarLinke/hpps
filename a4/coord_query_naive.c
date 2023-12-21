#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

#include "record.h"
#include "coord_query.h"

// calculates distance
double distance(const double lat_a, const double lon_a, const double lat_b, const double lon_b) {

  double result; 
  // the euclidian distance formula
  result = pow(lat_a-lat_b, 2) + pow(lon_a-lon_b, 2);

  if ( result == 0 ) {
    return result;
  }
  result = sqrt(result); 

  return result; 
}


struct naive_data {
  struct record *rs;
  int n;
};

struct naive_data* mk_naive(struct record* rs, int n) {
  struct naive_data* data = malloc(n*sizeof(struct record));
  data->rs = rs;
  data->n = n;
  return data;
}

void free_naive(struct naive_data* data) {
  for(int i = 0; i < data->n; i++){
    free(&data->rs[i]);
  }
  free(data);
}
const struct record* lookup_naive(struct naive_data* data, double lon, double lat) {
  // cuerry closest is instantiated as the distance from query to the first entry in the data.
  double current_closest = distance(data->rs[0].lat, data->rs[0].lon, lat, lon);
  int cc_index = 0;

  // instantiate once for good temporal locality 
  double tmp_lon;
  double tmp_lat;
  double query_dist;
 
  // iterate and compare between the remaining 1 upto n entries.
  for (int i = 1; i < data->n; i++) {
    tmp_lon = data->rs[i].lon;
    tmp_lat = data->rs[i].lat;
    query_dist = distance(tmp_lat, tmp_lon, lat, lon);
    if (query_dist < current_closest) {
      current_closest = query_dist;
      cc_index = i;
    }
  }
    struct record* match_record = &data->rs[cc_index];
  return match_record;

  
}

int main(int argc, char** argv) {
  return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_naive,
                          (free_index_fn)free_naive,
                          (lookup_fn)lookup_naive);
}
