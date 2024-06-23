#include "hash_table.h"

HashItem *CreateItem(uint32_t key, uint32_t value) {
  HashItem *item = (HashItem *)malloc(sizeof(HashItem));
  item->key = key;
  item->value = value;
  return item;
}

/* ----------------------------------------------------------------
 *      ChooseHashTableSize
 *
 *      Compute appropriate size for hashtable
 *      given the estimated number of rows
 * ----------------------------------------------------------------
 */
unsigned int ChooseHashTableSize(int rows) {
  // the coefficient for obtaining
  // the effective size of the hash table
  double fill_factor = 0.62;
  return pow(2, ceil(log2(rows / fill_factor)));
}

/* ----------------------------------------------------------------
 *		HashTableCreate
 *
 *		Create an empty hashtable data structure for hashjoin.
 * ----------------------------------------------------------------
 */
HashJoinTable *HashTableCreate(int rows) {
  HashJoinTable *table = (HashJoinTable *)malloc(sizeof(HashJoinTable));
  table->count = 0;
  table->size = ChooseHashTableSize(rows);
  table->items = (HashItem **)calloc(table->size, sizeof(HashItem *));
  for (unsigned int i = 0; i < table->size; ++i) table->items[i] = NULL;
  return table;
}

void HashTableInsert(HashJoinTable *hashtable, uint32_t key, uint32_t value) {
  HashItem *item = CreateItem(key, value);
  uint32_t hash_value;
  if (HashGetHashValue(hashtable, key, &hash_value)) {
    if (hashtable->count != hashtable->size) {
      if (!hashtable->items[hash_value]) {
        // inserting into a free cell
        hashtable->items[hash_value] = item;
        hashtable->count++;
      } else {
        // search for a free cell
        unsigned int hashtable_size = hashtable->size;
        while (hashtable->items[hash_value]) {
          hash_value++;
          hash_value %= hashtable_size;
        }
        if (!hashtable->items[hash_value]) hashtable->items[hash_value] = item;
      }
    } else
      printf("Table is full!");
  }
}

/*
 *      HashGetHashValue
 *
 *      Compute the hash value for a tuple
 *
 *      A true result means the hash value has been successfully computed
 *      and stored at *hashvalue.  A false result means the tuple cannot match
 *      because it contains a null attribute, and hence it should be discarded
 *      immediately
 */

bool HashGetHashValue(HashJoinTable *hashtable, uint32_t key,
                      uint32_t *hashvalue) {
  // if (key != MY_NULL) {
  //     const double kGoldenRatio = (sqrt(5) - 1) / 2;
  //     *hashvalue = (uint32_t)(hashtable->size * fmod(key * kGoldenRatio, 1));
  // }
  const double kGoldenRatio = (sqrt(5) - 1) / 2;
  *hashvalue = (uint32_t)(hashtable->size * fmod(key * kGoldenRatio, 1));
  return (true);
}

// unsigned int HashGetHashValue(uint32_t key, unsigned int hashtable_size) {
//     const double kGoldenRatio = (sqrt(5) - 1) / 2;
//     return (unsigned int)(hashtable_size * fmod(key * kGoldenRatio, 1));
// }

DynamicArray *SearchByKey(HashJoinTable *hashtable, uint32_t key) {
  uint32_t hash_value;
  DynamicArray *found_values = DynamicArrayCreate();
  if (HashGetHashValue(hashtable, key, &hash_value)) {
    unsigned int viewed_indexes = 0;
    unsigned int hashtable_size = hashtable->size;
    while (hashtable->items[hash_value] && viewed_indexes != hashtable_size) {
      if (hashtable->items[hash_value]->key == key)
        DynamicArrayInsert(found_values, hashtable->items[hash_value]->value);
      viewed_indexes++;
      hash_value++;
      hash_value %= hashtable_size;
    }
  }
  return found_values;
}

/* ----------------------------------------------------------------
 *		HashItemDestroy
 *
 *      Destroy the value in the hash table
 * ----------------------------------------------------------------
 */
void HashItemDestroy(HashItem *item) { free(item); }

/* ----------------------------------------------------------------
 *		HashTableDestroy
 *
 *      Destroy a hash table
 * ----------------------------------------------------------------
 */
void HashTableDestroy(HashJoinTable *table) {
  for (unsigned int i = 0; i < table->size; ++i)
    if (table->items[i]) HashItemDestroy(table->items[i]);
  free(table->items);
  free(table);
}
