#include "hash_table.h"

/* ----------------------------------------------------------------
 *		HashTableCreate
 *
 *		Create an empty hashtable data structure for hashjoin.
 * ----------------------------------------------------------------
 */
HashJoinTable *HashTableCreate(uint32_t rows) {
  HashJoinTable *table = (HashJoinTable *)malloc(sizeof(HashJoinTable));
  // if memory is allocated
  if (table) {
    table->size = ChooseHashTableSize(rows);
    table->count = 0;
    table->buckets = (HashBucket **)calloc(table->size, sizeof(HashBucket *));
    for (uint32_t i = 0; i < table->size; ++i) table->buckets[i] = NULL;
  }
  return table;
}

/* ----------------------------------------------------------------
 *		HashBucketNodeCreate
 *
 *		Create a hash table node with key and value.
 * ----------------------------------------------------------------
 */
HashBucketNode *HashBucketNodeCreate(int key, int value) {
  HashBucketNode *node = (HashBucketNode *)malloc(sizeof(HashBucketNode));
  node->key = key;
  node->value = value;
  return node;
}

/* ----------------------------------------------------------------
 *		HashBucketNodeInsert
 *
 *		Insert the created node into the bucket.
 * ----------------------------------------------------------------
 */
void HashBucketNodeInsert(HashJoinTable *hashtable, uint32_t hashvalue,
                          HashBucketNode *node) {
  // if the memory for the bucket is not allocated
  if (!hashtable->buckets[hashvalue]) {
    HashBucket *bucket = (HashBucket *)malloc(sizeof(HashBucket));
    bucket->nodes = (HashBucketNode **)malloc(sizeof(HashBucketNode *));
    bucket->size = 0;
    hashtable->buckets[hashvalue] = bucket;
  }
  HashBucket *bucket = hashtable->buckets[hashvalue];
  // if there are nodes in the bucket
  if (bucket->size)
    bucket->nodes = (HashBucketNode **)realloc(
        bucket->nodes, (bucket->size + 1) * sizeof(HashBucketNode *));
  bucket->nodes[bucket->size] = node;
  bucket->size++;
}

/* ----------------------------------------------------------------
 *      ChooseHashTableSize
 *
 *      Compute appropriate size for hashtable
 *      given the estimated number of rows
 * ----------------------------------------------------------------
 */
uint32_t ChooseHashTableSize(uint32_t rows) {
  /* the coefficient for obtaining
   *  the effective size of the hash table
   */
  double fill_factor = 0.62;
  return pow(2, ceil(log2(rows / fill_factor)));
}

/* ----------------------------------------------------------------
 *		HashTableInsert
 *
 *		Insert the node into the hash table.
 *    Collisions are resolved using a list of nodes.
 *
 *    If the key is NULL, no insertion
 *    into the hash table is performed
 * ----------------------------------------------------------------
 */
void HashTableInsert(HashJoinTable *hashtable, int key, int value) {
  uint32_t hashvalue;
  // if key = Null, hashvalue will not be received
  if (GetHashValue(hashtable, key, &hashvalue)) {
    // checking the completeness of the hash table
    if (hashtable->count != hashtable->size) {
      HashBucketNode *node = HashBucketNodeCreate(key, value);
      HashBucketNodeInsert(hashtable, hashvalue, node);
    } else
      printf("Table is full!");
  }
}

/* ----------------------------------------------------------------
 *      GetHashValue
 *
 *      Compute the hash value for a tuple
 *
 *      A true result means the hash value has been
 *      successfully computed and stored at *hashvalue.
 *
 *      A false result means the tuple cannot match
 *      because it contains a null attribute,
 *      and hence it should be discarded immediately.
 */
bool GetHashValue(HashJoinTable *hashtable, int key, uint32_t *hashvalue) {
  bool is_null = IsNull(key);
  if (!is_null) {
    const double kGoldenRatio = (sqrt(5) - 1) / 2;
    *hashvalue = (uint32_t)(hashtable->size * fmod(key * kGoldenRatio, 1));
  }
  return !is_null;
}

/* ----------------------------------------------------------------
 *    SearchByKey
 *
 *    Searching for values corresponding to the key
 *
 *    1. Calculate the hashvalue of the key
 *    2. Determine the bucket corresponding to the hashvalue
 *    3. View the list of nodes in the bucket
 * ----------------------------------------------------------------
 */
DynamicArray *SearchByKey(HashJoinTable *hashtable, int key) {
  uint32_t hashvalue;
  // the values corresponding to the key are stored in a dynamic array
  DynamicArray *found_values = DynamicArrayCreate();
  if (GetHashValue(hashtable, key, &hashvalue)) {
    HashBucket *bucket = hashtable->buckets[hashvalue];
    for (uint32_t i = 0; i < bucket->size; ++i)
      if (bucket->nodes[i]->key == key)
        DynamicArrayInsert(found_values, bucket->nodes[i]->value);
  }
  return found_values;
}

/* ----------------------------------------------------------------
 *		HashItemDestroy
 *
 *    Destroy the value in the hash table
 * ----------------------------------------------------------------
 */
void HashBucketNodeDestroy(HashBucketNode *node) { free(node); }

/* ----------------------------------------------------------------
 *		HashTableDestroy
 *
 *    Destroy a hash table
 * ----------------------------------------------------------------
 */
void HashTableDestroy(HashJoinTable *table) {
  for (uint32_t i = 0; i < table->size; ++i)
    if (table->buckets[i]) {
      for (uint32_t j = 0; j < table->buckets[i]->size; ++j)
        HashBucketNodeDestroy(table->buckets[i]->nodes[j]);
      free(table->buckets[i]->nodes);
      free(table->buckets[i]);
    }
  free(table->buckets);
  free(table);
}
