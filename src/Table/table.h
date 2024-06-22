#ifndef SRC_TABLE_TABLE_H
#define SRC_TABLE_TABLE_H

#include <memory.h>
#include <stdlib.h>

typedef struct Row {
    int a;
    int b;
} Row;

typedef struct Table {
    Row** rows;
    unsigned int size;
} Table;


Table* CreateTable(int rows);
void InsertIntoRow(Table *table, unsigned int row_number, int a, int b);
void FreeRow(Row *row);
void FreeTable(Table *table);

#endif  // SRC_TABLE_TABLE_H