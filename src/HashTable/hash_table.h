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

#include "../DynamicArray/dynamic_array.h"
#include "../Table/table.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct HashNode {
    int key;
    int value;
} HashNode;

typedef struct HashJoinTable {
    HashNode** items;
    uint32_t size;
    uint32_t count;
} HashJoinTable;

// the function of creating a hash table node
HashNode* HashNodeCreate(int key, int value);

// the function for calculating the number of buckets in the hash table
uint32_t ChooseHashTableSize(uint32_t rows);

// the function of creating a hash table
HashJoinTable* HashTableCreate(uint32_t rows);

// hash table node insertion function
void HashTableInsert(HashJoinTable* hashtable, int key, int value);
bool HashGetHashValue(HashJoinTable* hashtable, int key, uint32_t *hashvalue);

DynamicArray *SearchByKey(HashJoinTable *table, int key);

// memory release functions
void HashItemDestroy(HashNode* item);
void HashTableDestroy(HashJoinTable* table);


#endif  // SRC_HASHTABLE_HASH_TABLE_H