#include "hash_table.h"

HashItem *CreateItem(int key, int value) {
    HashItem *item = (HashItem *)malloc(sizeof(HashItem));
    item->key = key;
    item->value = value;
    return item;
}

unsigned int ChooseHashTableSize(int elements) {
    // the coefficient for obtaining
    // the effective size of the hash table
    double fill_factor = 0.62;
    return pow(2, ceil(log2(elements / fill_factor)));
}

HashTable *CreateHashTable(int elements) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->count = 0;
    table->size = ChooseHashTableSize(elements);
    table->items = (HashItem **)calloc(table->size, sizeof(HashItem *));
    for (unsigned int i = 0; i < table->size; ++i)
        table->items[i] = NULL;
    return table;
}

void HashTableInsert(HashTable *hashtable, int key, int value) {
    HashItem *item = CreateItem(key, value);
    unsigned int hash_index = GetHashValue(key, hashtable->size);
    if (hashtable->count != hashtable->size) {
        if (!hashtable->items[hash_index]) {
            // inserting into a free cell
            hashtable->items[hash_index] = item;
            hashtable->count++;
        }
        else {
            // search for a free cell
            unsigned int hashtable_size = hashtable->size;
            while (hashtable->items[hash_index]) {
                hash_index++;
                hash_index %= hashtable_size;
            }
            if (!hashtable->items[hash_index])
                hashtable->items[hash_index] = item;
        }
    }
    else
        printf("Table is full!");
}

unsigned int GetHashValue(int key, unsigned int hashtable_size) {
    const double kGoldenRatio = (sqrt(5) - 1) / 2;
    return (unsigned int)(hashtable_size * fmod(key * kGoldenRatio, 1));
}

DynamicArray *SearchByKey(HashTable *table, int key) {
    unsigned int hash_index = GetHashValue(key, table->size);
    unsigned int viewed_indexes = 0;
    unsigned int hashtable_size = table->size;

    DynamicArray *found_values = CreateDynamicArray();
    while (table->items[hash_index] && viewed_indexes != hashtable_size) {
        if (table->items[hash_index]->key == key)
            InsertIntoDynamicArray(found_values, table->items[hash_index]->value);
        viewed_indexes++;
        hash_index++;
        hash_index %= hashtable_size;
    }
    return found_values;
}

void FreeHashItem(HashItem *item) {
    free(item);
}

void FreeHashTable(HashTable *table) {
    for (unsigned int i = 0; i < table->size; ++i)
        if (table->items[i])
            FreeHashItem(table->items[i]);
    free(table->items);
    free(table);
}
