#include "hash_join.h"

#define WORK_MEM 4000000

/* ----------------------------------------------------------------
 *		HashJoinImpl
 *      
 *      1. Calculate the number of batches;
 *      2. Create a hash table from a smaller table;
 *      3. Sequentially scan a larger table;
 *      4. Create a table with the result set;
 *
 *		Note: the relation we build hash table on is the "inner"
 *			  the other one is "outer".
 *
 * (ExecHashJoinImpl
 *    https://github.com/postgres/postgres/blob/REL_15_2/src/backend/executor/nodeHashjoin.c)
 * ----------------------------------------------------------------
 */
Table *HashJoinImpl(Table *t1, Table *t2) {
  Table *inner, *outer;
  if (t1->size < t2->size) {
    outer = t2;
    inner = t1;
  } else {
    outer = t1;
    inner = t2;
  }
  // number of batches
  uint32_t nbatch = HashIncreaseNumBatches(inner->size);
  // number of buckets per batch
  uint32_t nbatch_buckets = ceil((float)inner->size / nbatch);
  
  // table for fields from tables t1 and t2
  Table *join_table = TableCreate(t1->fields_count + t2->fields_count);
  
  // passage through all batches
  for (uint32_t k = 0; k < nbatch; ++k) {
    // create a hashtable with a size corresponding to one batch
    HashJoinTable *hashtable = HashTableCreate(nbatch_buckets);

    // fill the hashtable with values from the table
    uint32_t start_bucket = nbatch_buckets * k;
    uint32_t stop_bucket = start_bucket + nbatch_buckets;
    stop_bucket = (stop_bucket < inner->size) ? stop_bucket : inner->size;
    for (uint32_t i = start_bucket; i < stop_bucket; ++i)
      HashTableInsert(hashtable, inner->rows[i]->fields[0],
                    inner->rows[i]->fields[1]);

    // sequential passage through a larger table
    for (uint32_t i = 0; i < outer->size; ++i) {
      int hashtable_key = outer->rows[i]->fields[0];
      // searching for a key in a hashtable
      DynamicArray *found_values = SearchByKey(hashtable, hashtable_key);
      // inserting the found values into the resulting table
      uint32_t a = outer->rows[i]->fields[0];
      uint32_t b = outer->rows[i]->fields[1];
      for (uint32_t j = 0; j < found_values->size; ++j) {
        int fields[4] = {a, b, a, found_values->values[j]};
        TableInsert(join_table, fields);
      }
      DynamicArrayDestroy(found_values);
    }
    HashTableDestroy(hashtable);
  }
  return join_table;
}


/* ----------------------------------------------------------------
 * HashIncreaseNumBatches
 *		increase the original number of batches in order to reduce
 *		current memory consumption
 *
 * (ExecHashIncreaseNumBatches
 *    https://github.com/postgres/postgres/blob/REL_15_2/src/backend/executor/nodeHash.c)
 * ----------------------------------------------------------------
 */
uint32_t HashIncreaseNumBatches(float size) {
  return ceil(size * sizeof(HashBucket) / WORK_MEM);
}

void HashJoinOutput(Table *join_table, const char *filename) {
  FILE *file = fopen(filename, "w");
  fprintf(file, "|----------------------------------|\n");
  fprintf(file, "|  t1.a ||  t1.b ||  t2.a ||  t2.b |\n");
  fprintf(file, "|----------------------------------|\n");

  for (uint32_t i = 0; i < join_table->size; ++i) {
    for (uint32_t j = 0; j < join_table->fields_count; ++j)
      fprintf(file, "| %5d |", join_table->rows[i]->fields[j]);
    fprintf(file, "\n");
  }
  fprintf(file, "|----------------------------------|\n");

  fclose(file);
}

int main() {
  Table *t1 = TableCreate(2);
  Table *t2 = TableCreate(2);

  for (int i = 1; i <= 10000; ++i) {
    int fields[2] = {i, i % 100};
    TableInsert(t1, fields);
    TableInsert(t2, fields);
  }

  Table *join_table = HashJoinImpl(t1, t2);
  HashJoinOutput(join_table, "output.txt");

  TableDestroy(t1);
  TableDestroy(t2);
  TableDestroy(join_table);
}