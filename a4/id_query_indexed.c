#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include "record.h"
#include "id_query.h"

struct index_record {
  int64_t osm_id;
  const struct record* record;
};

struct indexed_data{ 
    struct index_record* irs; 
    int n;
};

struct indexed_data* mk_indexed(struct record* rs, int n) {
    struct index_record* first_ir = malloc(n * sizeof(struct index_record)); 
    for (int i = 0; i < n; i++){ 
      first_ir[i].osm_id = rs[i].osm_id; 
      first_ir[i].record = &rs[i];
    }

struct indexed_data* data = malloc(sizeof(struct indexed_data)); 
data->n = n; 
data->irs = first_ir;
return data;
}

void free_indexed(struct indexed_data* data) {
  // Free space for each of the records we've malloced for. We malloced n*index_record and indexed_data.
  for (int i = 0; i < data->n; i++) {
    free(&data->irs[i]);
  }
  free(data);

}

const struct record* lookup_indexed(struct indexed_data* data, int64_t needle) {

  for (int i = 0; i < data->n; i++){ 

    if(needle == data->irs[i].osm_id){ 
      return data->irs[i].record; 
    }

  }
  return NULL;

}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_indexed,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_indexed);
}
