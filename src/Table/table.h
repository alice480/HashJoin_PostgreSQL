#ifndef SRC_TABLE_TABLE_H
#define SRC_TABLE_TABLE_H

#include <memory.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Row {
    uint32_t *fields;
} Row;

typedef struct Table {
    Row** rows;
    uint32_t fields_count;
    uint32_t size;
} Table;

Table* TableCreate(uint32_t fields);
void TableInsert(Table *table, uint32_t *fields);
void RowDestroy(Row *row);
void TableDestroy(Table *table);

#endif  // SRC_TABLE_TABLE_H