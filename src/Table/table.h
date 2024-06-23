#ifndef SRC_TABLE_TABLE_H
#define SRC_TABLE_TABLE_H

#include <memory.h>
#include <stdlib.h>

typedef struct Row {
    int *fields;
} Row;

typedef struct Table {
    Row** rows;
    unsigned int fields_count;
    unsigned int size;
} Table;

Table* CreateTable(int fields);
void InsertIntoTable(Table *table, int *fields);
void FreeRow(Row *row);
void FreeTable(Table *table);

#endif  // SRC_TABLE_TABLE_H