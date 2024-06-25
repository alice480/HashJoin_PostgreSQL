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
    for (uint32_t i = 0; i < table->size; ++i)
      table->buckets[i] = NULL;
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

void HashBucketNodeInsert(HashJoinTable* hashtable, uint32_t hashvalue, HashBucketNode* node) {
  // if the memory for the bucket is not allocated
  if (!hashtable->buckets[hashvalue]) {
    hashtable->buckets[hashvalue] = (HashBucket *)malloc(sizeof(HashBucket));
    hashtable->buckets[hashvalue]->nodes = (HashBucketNode **)malloc(sizeof(HashBucketNode *));
    hashtable->buckets[hashvalue]->size = 0;
  }
  // if there are nodes in the bucket
  uint32_t bucket_size = hashtable->buckets[hashvalue]->size;
  if (bucket_size) {
    hashtable->buckets[hashvalue]->nodes = (HashBucketNode **)realloc(hashtable->buckets[hashvalue]->nodes, (bucket_size + 1) * sizeof(HashBucketNode *));
  }
  hashtable->buckets[hashvalue]->nodes[hashtable->buckets[hashvalue]->size] = node;
  hashtable->buckets[hashvalue]->size++;
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
 *		Inserting a node into a hashtable.
 *    Collisions are solved by using 
 *    open addressing and linear probing.
 * 
 *    If the key is NULL, no insertion 
 *    into the hash table is performed
 * ----------------------------------------------------------------
 */
void HashTableInsert(HashJoinTable *hashtable, int key, int value) {
  uint32_t hash_value;
  // if key = Null, hashvalue will not be received
  if (GetHashValue(hashtable, key, &hash_value)) {
    // checking the completeness of the hash table
    if (hashtable->count != hashtable->size) {
      HashBucketNode *node = HashBucketNodeCreate(key, value);
      HashBucketNodeInsert(hashtable, hash_value, node);
    } else
      printf("Table is full!");
  }
}

/*
 *      GetHashValue
 *
 *      Compute the hash value for a tuple
 *
 *      A true result means the hash value has been successfully computed
 *      and stored at *hashvalue.  A false result means the tuple cannot match
 *      because it contains a null attribute, and hence it should be discarded
 *      immediately
 */
bool GetHashValue(HashJoinTable *hashtable, int key,
                      uint32_t *hashvalue) {
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
 *    1. The hash_value of the key is calculated
 *    2. HashBuckets from hash_value to the first NULL are searched
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
DynamicArray *SearchByKey(HashJoinTable *hashtable, int key) {
  uint32_t hash_value;
  // the values corresponding to the key are stored in a dynamic array
  DynamicArray *found_values = DynamicArrayCreate();
  if (GetHashValue(hashtable, key, &hash_value)) {
    HashBucket *bucket = hashtable->buckets[hash_value];
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
