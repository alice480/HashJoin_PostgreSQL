/*-------------------------------------------------------------------------
 *
 * nodeHash.h
 *	  prototypes for hash_table.c
 *
 *
 * src/HashTable/hash_table.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef SRC_HASHTABLE_HASH_TABLE_H
#define SRC_HASHTABLE_HASH_TABLE_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../DynamicArray/dynamic_array.h"
#include "../Table/table.h"

typedef struct HashBucketNode {
  int key;
  int value;
} HashBucketNode;

typedef struct HashBucket {
  HashBucketNode **nodes;
  uint32_t size;
} HashBucket;

typedef struct HashJoinTable {
  HashBucket **buckets;
  uint32_t nbuckets;
  uint32_t count;
} HashJoinTable;

// structure creation functions
HashJoinTable *HashTableCreate(uint32_t rows);
HashBucketNode *HashBucketNodeCreate(int key, int value);

// functions of insertion into structures
void HashBucketNodeInsert(HashJoinTable *hashtable, uint32_t hashvalue,
                          HashBucketNode *node);
void HashTableInsert(HashJoinTable *hashtable, int key, int value);

uint32_t ChooseHashTableSize(uint32_t rows);
bool GetHashValue(HashJoinTable *hashtable, int key, uint32_t *hashvalue);
DynamicArray *SearchByKey(HashJoinTable *table, int key);

// memory release functions
void HashBucketNodeDestroy(HashBucketNode *node);
void HashTableDestroy(HashJoinTable *table);

#endif  // SRC_HASHTABLE_HASH_TABLE_H