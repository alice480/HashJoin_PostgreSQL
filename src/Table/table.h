/*-------------------------------------------------------------------------
 *
 * table.h
 *	  prototypes for table.c
 *
 *
 * src/table/table.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef SRC_TABLE_TABLE_H
#define SRC_TABLE_TABLE_H

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Row {
  int *fields;
} Row;

typedef struct Table {
  Row **rows;
  uint32_t fields_count;
  uint32_t size;
} Table;

Table *TableCreate(uint32_t fields);
void TableInsert(Table *table, int *fields);
void RowDestroy(Row *row);
void TableDestroy(Table *table);

bool IsNull(int field);

#endif  // SRC_TABLE_TABLE_H