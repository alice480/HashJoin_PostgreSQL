#ifndef SRC_HASHTABLE_HASH_TABLE_H
#define SRC_HASHTABLE_HASH_TABLE_H

#include "../DynamicArray/dynamic_array.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct HashItem {
    int key;
    int value;
} HashItem;

typedef struct HashTable {
    HashItem** items;
    unsigned int size;
    unsigned int count;
} HashTable;

// the function of creating a hash table element
HashItem* CreateItem(int key, int value);

// the function for calculating the number of buckets in the hash table
unsigned int ChooseHashTableSize(int elements);

// the function of creating a hash table
HashTable* CreateHashTable(int elements);

// memory release functions
void FreeHashItem(HashItem* item);
void FreeHashTable(HashTable* table);

// void HandleCollision(HashTable* table, HashItem* item);
void HashTableInsert(HashTable* hashtable, int key, int value);
unsigned int GetHashValue(int key, unsigned int hashtable_size);

DynamicArray *SearchByKey(HashTable *table, int key);


#endif  // SRC_HASHTABLE_HASH_TABLE_H