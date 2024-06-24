#include "hash_table.h"

/* ----------------------------------------------------------------
 *      HashNodeCreate
 *
 *      Create a hashtable node with specific values.
 * ----------------------------------------------------------------
 */
HashNode *HashNodeCreate(uint32_t key, uint32_t value) {
  HashNode *node = (HashNode *)malloc(sizeof(HashNode));
  node->key = key;
  node->value = value;
  return node;
}

/* ----------------------------------------------------------------
 *      ChooseHashTableSize
 *
 *      Compute appropriate size for hashtable
 *      given the estimated number of rows
 * ----------------------------------------------------------------
 */
uint32_t ChooseHashTableSize(int rows) {
  /* the coefficient for obtaining
  *  the effective size of the hash table
  */
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
  table->items = (HashNode **)calloc(table->size, sizeof(HashNode *));
  for (uint32_t i = 0; i < table->size; ++i) 
    table->items[i] = NULL;
  return table;
}


/* ----------------------------------------------------------------
 *		HashTableInsert
 *
 *		Inserting a node into a hashtable.
 *    Collisions are solved by using 
 *    open addressing and linear probing.
 * 
 *    If the key is NULL, no insertion 
 *    into the hash table is performed
 * ----------------------------------------------------------------
 */
void HashTableInsert(HashJoinTable *hashtable, uint32_t key, uint32_t value) {
  HashNode *node = HashNodeCreate(key, value);
  uint32_t hash_value;
  if (HashGetHashValue(hashtable, key, &hash_value)) {
    if (hashtable->count != hashtable->size) {
      if (!hashtable->items[hash_value]) {
        // inserting into a free cell
        hashtable->items[hash_value] = node;
        hashtable->count++;
      } else {
        // search for a free cell
        uint32_t hashtable_size = hashtable->size;
        while (hashtable->items[hash_value]) {
          hash_value++;
          hash_value %= hashtable_size;
        }
        if (!hashtable->items[hash_value]) hashtable->items[hash_value] = node;
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


/* ----------------------------------------------------------------
 *    SearchByKey
 *
 *    Searching for values corresponding to the key
 * 
 *    1. The hash_value of the key is calculated
 *    2. HashNodes from hash_value to the first NULL are searched
 *      
 *    Note: NULL means that the following cells 
 *          contain values that do not satisfy the key.
 *          In the worst case, if the entire hash table is full, 
 *          all hashes will be viewed.
 * 
 *    3. The found values are entered into a DynamicArray, 
 *       which will be returned from the function
 * ----------------------------------------------------------------
 */
DynamicArray *SearchByKey(HashJoinTable *hashtable, uint32_t key) {
  uint32_t hash_value;
  // the values corresponding to the key are stored in a dynamic array
  DynamicArray *found_values = DynamicArrayCreate();
  if (HashGetHashValue(hashtable, key, &hash_value)) {
    uint32_t viewed_indexes = 0;
    uint32_t hashtable_size = hashtable->size;
    // viewing from the hash_value to the first NULL
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
 *    Destroy the value in the hash table
 * ----------------------------------------------------------------
 */
void HashItemDestroy(HashNode *item) { free(item); }

/* ----------------------------------------------------------------
 *		HashTableDestroy
 *
 *    Destroy a hash table
 * ----------------------------------------------------------------
 */
void HashTableDestroy(HashJoinTable *table) {
  for (uint32_t i = 0; i < table->size; ++i)
    if (table->items[i])
      HashItemDestroy(table->items[i]);
  free(table->items);
  free(table);
}
