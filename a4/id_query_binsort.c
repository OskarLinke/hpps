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

int bin_cmpfunc(const void *key, const void *irs){
    int64_t search_id = *((int64_t *)key);
    const struct index_record *record = (const struct index_record *)irs;
    
    if (search_id < record->osm_id) return -1;
    else if (search_id > record->osm_id) return 1;
    else return 0;
}

int cmpfunc(const void *a, const void *b) {
    const struct index_record *record_a = (const struct index_record *)a;
    const struct index_record *record_b = (const struct index_record *)b;
    if (record_a->osm_id < record_b->osm_id) return -1;
    else if (record_a->osm_id > record_b->osm_id) return 1;
    else return 0;
}


struct indexed_data* mk_indexed(struct record* rs, int n) {
    struct index_record* irs = malloc(n * sizeof(struct index_record)); 
    for (int i = 0; i < n; i++){ 
      irs[i].osm_id = rs[i].osm_id; 
      irs[i].record = &rs[i];
    }
    if (irs == NULL) {
        printf("irs == NULL \n");
        return NULL;
    }
    qsort(irs, n, sizeof(struct index_record), cmpfunc);

    struct indexed_data* data = malloc(sizeof(struct indexed_data)); 
    data->n = n; 
    data->irs = irs;
    return data;
}

void free_indexed(struct indexed_data* data) {
  for(int i = 0; i< data->n; i++){ 
    free(&data->irs[i].record[0]);
    free(&data->irs[i]);
  }

  free(data);
}

const struct record* lookup_indexed(struct indexed_data* data, int64_t needle) {

  struct index_record* hit;
  hit  = (struct index_record*) bsearch(&needle, data->irs, data->n, sizeof (struct index_record), bin_cmpfunc);
  if( hit != NULL ) {
      return hit->record;
   } else {
      return NULL;
   }

}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_indexed,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_indexed);
}
