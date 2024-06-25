#include "hash_join.h"

/* ----------------------------------------------------------------
 *		HashJoinImpl
 *
 *      1. Create a hash table from a smaller table;
 *      2. Sequentially scan a larger table;
 *      3. Create a table with the result set
 *
 *		Note: the relation we build hash table on is the "inner"
 *			  the other one is "outer".
 * ----------------------------------------------------------------
 */
Table *HashJoinImpl(Table *t1, Table *t2) {
  HashJoinTable *hashtable;
  Table *inner, *outer;
  // choosing a smaller table to create a hash table
  if (t1->size < t2->size) {
    // build hash table for inner relation
    hashtable = HashTableCreate(t1->size);
    outer = t2;
    inner = t1;
  } else {
    // build hash table for inner relation
    hashtable = HashTableCreate(t2->size);
    outer = t1;
    inner = t2;
  }
  // fill the hashtable with values from the table
  for (uint32_t i = 0; i < inner->size; ++i)
    HashTableInsert(hashtable, inner->rows[i]->fields[0],
                    inner->rows[i]->fields[1]);

  SearchByKey(hashtable, 1);

  // table for fields from tables t1 and t2
  Table *join_table = TableCreate(t1->fields_count + t2->fields_count);
  // sequential passage through a larger table
  for (uint32_t i = 0; i < outer->size; ++i) {
    // check for the presence of the key in the hashtable
    int hashtable_key = outer->rows[i]->fields[0];
    DynamicArray *found_values = SearchByKey(hashtable, hashtable_key);
    uint32_t a = outer->rows[i]->fields[0];
    uint32_t b = outer->rows[i]->fields[1];
    for (uint32_t j = 0; j < found_values->size; ++j) {
      int fields[4] = {a, b, a, found_values->values[j]};
      TableInsert(join_table, fields);
    }
    DynamicArrayDestroy(found_values);
  }
  HashTableDestroy(hashtable);
  return join_table;
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