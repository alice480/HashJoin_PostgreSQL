#include "hash_table.h"

HashItem* CreateItem(int key, int value) {
    HashItem* item = (HashItem*)malloc(sizeof(HashItem));
    item->key = key;
    item->value = value;
    return item;
}

unsigned int CalculateSizeOfHashTable(int elements) {
    // the coefficient for obtaining 
    // the effective size of the hash table
    double fill_factor = 0.62;
    return pow(2, ceil(log2(elements / fill_factor)));
}

HashTable* CreateHashTable(int elements) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->count = 0;
    table->size = CalculateSizeOfHashTable(elements);
    table->items = (HashItem**)calloc(table->size, sizeof(HashItem*));
    for (unsigned int i = 0; i < table->size; ++i)
        table->items[i] = NULL;
    return table;
}


void InsertIntoHashTable(HashTable* table, int key, int value) {
    HashItem* item = CreateItem(key, value);
    unsigned int hash_index = CalculateMultiplicativeHashFunc(key, table->size);
    if (table->count != table->size) {
        if (!table->items[hash_index]) {
            // inserting into a free cell
            table->items[hash_index] = item;
            table->count++;
        } else
            if (table->items[hash_index]->key == key)
                // update the item
                table->items[hash_index]->value = value;
            else {
                // search for a free cell
                unsigned int hash_table_size = table->size;
                while (table->items[hash_index]) {
                    hash_index++;
                    hash_index %= hash_table_size;
                }
                if (!table->items[hash_index])
                    table->items[hash_index] = item;
            }
    } else {
        printf("Table is full!");
    }
}

unsigned int CalculateMultiplicativeHashFunc(int key, unsigned int hash_table_size) {
    const double kGoldenRatio = (sqrt(5) - 1) / 2;
    return (unsigned int)(hash_table_size * fmod(key * kGoldenRatio, 1));
}

int SearchByKey(HashTable* table, int key) {
    unsigned int hash_index = CalculateMultiplicativeHashFunc(key, table->size);
    while (table->items[hash_index]) {
        if (table->items[hash_index]->key == key)
            return table->items[hash_index]->value;
    }
    return -1;
}

void FreeHashItem(HashItem* item) {
    free(item);
}

void FreeHashTable(HashTable* table) {
    for (unsigned int i = 0; i < table->size; ++i)
        if (table->items[i])
            FreeHashItem(table->items[i]);

    free(table->items);
    free(table);
}

int main() {
    HashTable* hash_table = CreateHashTable(10000);
    
    for (int i = 1; i <= 10000; ++i)
        InsertIntoHashTable(hash_table, i, i % 100);

    for (int i = 1; i <= 10000; ++i) {
        int value_in_table = SearchByKey(hash_table, i);
        if (value_in_table != i % 100)
            printf("FAIL: key - %d, value_in_table - %d\n", i, value_in_table);
    }

    
    FreeHashTable(hash_table);
}