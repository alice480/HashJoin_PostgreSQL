#ifndef SRC_HASHTABLE_HASH_TABLE_H
#define SRC_HASHTABLE_HASH_TABLE_H

#include "../DynamicArray/dynamic_array.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MY_NULL 4294967296

typedef struct HashItem {
    uint32_t key;
    uint32_t value;
} HashItem;

typedef struct HashJoinTable {
    HashItem** items;
    unsigned int size;
    unsigned int count;
} HashJoinTable;

// the function of creating a hash table element
HashItem* CreateItem(uint32_t key, uint32_t value);

// the function for calculating the number of buckets in the hash table
unsigned int ChooseHashTableSize(int rows);

// the function of creating a hash table
HashJoinTable* HashTableCreate(int rows);

// void HandleCollision(HashTable* table, HashItem* item);
void HashTableInsert(HashJoinTable* hashtable, uint32_t key, uint32_t value);
bool HashGetHashValue(HashJoinTable* hashtable, uint32_t key, uint32_t *hashvalue);

DynamicArray *SearchByKey(HashJoinTable *table, uint32_t key);

// memory release functions
void HashItemDestroy(HashItem* item);
void HashTableDestroy(HashJoinTable* table);


#endif  // SRC_HASHTABLE_HASH_TABLE_H