#include "hash_join.h"

void HashJoin(Table *t1, Table *t2) {
    HashTable *hash_table;
    Table *table_for_seq_scan, *table_for_hash_table;
    if (t1->size < t2->size) {
        hash_table = CreateHashTable(t1->size);
        table_for_seq_scan = t2;
        table_for_hash_table = t1;
    } else {
        hash_table = CreateHashTable(t2->size);
        table_for_seq_scan = t1;
        table_for_hash_table = t2;
    }

    // filling the hash table with values from the table
    for (unsigned int i = 0; i < table_for_hash_table->size; ++i) {
        InsertIntoHashTable(hash_table, table_for_hash_table->rows[i]->a, table_for_hash_table->rows[i]->b);
    }

    for (unsigned int i = 0; i < table_for_seq_scan->size; ++i) {
        DynamicArray *array = SearchByKey(hash_table, table_for_seq_scan->rows[i]->a);
        int a = table_for_seq_scan->rows[i]->a;
        int b = table_for_hash_table->rows[i]->b;
        for (unsigned int j = 0;  j < array->size; ++j) {
            printf("|  %d  |  %d  |  %d  |  %d  |\n", a, b, a, array->values[j]);
        }
        FreeDynamicArray(array);
    }

    FreeHashTable(hash_table);   
}

int main() {
    Table *t1 = CreateTable(10000);
    Table *t2 = CreateTable(10000);

    for (unsigned int i = 0; i < 10000; ++i) {
        InsertIntoRow(t1, i, i, i % 100);
        InsertIntoRow(t2, i, i, i % 100);
    }

    HashJoin(t1, t2);

    FreeTable(t1);
    FreeTable(t2);
}