#include "hash_join.h"


/* ----------------------------------------------------------------
 *		HashJoinImpl
 *      
 *      1. Creating a hash table from a smaller table.
 *      2. Sequential scanning of a larger table
 *      3. Creating a table with a result set
 *
 *		Note: the relation we build hash table on is the "inner"
 *			  the other one is "outer".
 * ----------------------------------------------------------------
 */
Table *HashJoinImpl(Table *t1, Table *t2) {
    HashJoinTable *hashtable;
    Table *inner, *outer;
    // choosing a smaller table to create a hash table
    if (t1->size < t2->size) {
        hashtable = HashTableCreate(t1->size);
        outer = t2;
        inner = t1;
    } else {
        hashtable = HashTableCreate(t2->size);
        outer = t1;
        inner = t2;
    }
    // filling the hash table with values from the table
    for (unsigned int i = 0; i < inner->size; ++i)
        HashTableInsert(hashtable, 
                        inner->rows[i]->fields[0], 
                        inner->rows[i]->fields[1]);

    // table for fields from tables t1 and t2
    Table *join_table = TableCreate(t1->fields_count + t2->fields_count);
    // sequential passage through a larger table
    for (unsigned int i = 0; i < outer->size; ++i) {
        // checking for the key in the hash table
        int hashtable_key = outer->rows[i]->fields[0];
        DynamicArray *found_values = SearchByKey(hashtable, hashtable_key);
        int a = outer->rows[i]->fields[0];
        int b = outer->rows[i]->fields[1];
        for (unsigned int j = 0;  j < found_values->size; ++j) {
            int fields[4] = {a, b, a, found_values->values[j]};
            TableInsert(join_table, fields);
        }
        DynamicArrayDestroy(found_values);
    }
    HashTableDestroy(hashtable);   
    return join_table;
}

int main() {
    Table *t1 = TableCreate(2);
    Table *t2 = TableCreate(2);

    for (unsigned int i = 0; i < 10000; ++i) {
        int fields[2] = {i, i % 100};
        TableInsert(t1, fields);
        TableInsert(t2, fields);
    }

    Table *join_table = HashJoinImpl(t1, t2);

    for (unsigned int i = 0; i < join_table->size; ++i) {
        for (unsigned int j = 0; j < join_table->fields_count; ++j)
            printf("|  %d  |", join_table->rows[i]->fields[j]);
        printf("\n");
    }

    TableDestroy(t1);
    TableDestroy(t2);
    TableDestroy(join_table);
}