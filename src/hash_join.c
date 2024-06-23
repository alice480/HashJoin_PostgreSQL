#include "hash_join.h"

Table *HashJoin(Table *t1, Table *t2) {
    HashTable *hashtable;
    Table *table_for_seq_scan, *table_for_hashtable;
    // choosing a smaller table to create a hash table
    if (t1->size < t2->size) {
        hashtable = CreateHashTable(t1->size);
        table_for_seq_scan = t2;
        table_for_hashtable = t1;
    } else {
        hashtable = CreateHashTable(t2->size);
        table_for_seq_scan = t1;
        table_for_hashtable = t2;
    }
    // filling the hash table with values from the table
    for (unsigned int i = 0; i < table_for_hashtable->size; ++i)
        HashTableInsert(hashtable, 
                        table_for_hashtable->rows[i]->fields[0], 
                        table_for_hashtable->rows[i]->fields[1]);

    // table for fields from tables t1 and t2
    Table *join_table = CreateTable(t1->fields_count + t2->fields_count);
    // sequential passage through a larger table
    for (unsigned int i = 0; i < table_for_seq_scan->size; ++i) {
        // checking for the key in the hash table
        int hashtable_key = table_for_seq_scan->rows[i]->fields[0];
        // printf("Key: %d", hashtable_key);
        DynamicArray *found_values = SearchByKey(hashtable, hashtable_key);
        int a = table_for_seq_scan->rows[i]->fields[0];
        int b = table_for_hashtable->rows[i]->fields[1];
        // printf("%d\n", found_values->size);
        for (unsigned int j = 0;  j < found_values->size; ++j) {
            int fields[4] = {a, b, a, found_values->values[j]};
            InsertIntoTable(join_table, fields);
        }
        FreeDynamicArray(found_values);
    }
    FreeHashTable(hashtable);   
    return join_table;
}

int main() {
    Table *t1 = CreateTable(2);
    Table *t2 = CreateTable(2);

    for (unsigned int i = 0; i < 10000; ++i) {
        int fields[2] = {i, i % 100};
        InsertIntoTable(t1, fields);
        InsertIntoTable(t2, fields);
    }

    Table *join_table = HashJoin(t1, t2);

    for (unsigned int i = 0; i < join_table->size; ++i) {
        for (unsigned int j = 0; j < join_table->fields_count; ++j)
            printf("|  %d  |", join_table->rows[i]->fields[j]);
        printf("\n");
    }

    FreeTable(t1);
    FreeTable(t2);
    FreeTable(join_table);
}