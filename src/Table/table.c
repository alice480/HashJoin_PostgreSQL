#include "table.h"

Table *TableCreate(uint32_t fields) {
  Table *table = (Table *)malloc(sizeof(Table));
  table->rows = (Row **)malloc(sizeof(Row *));
  table->size = 0;
  table->fields_count = fields;
  return table;
}

void TableInsert(Table *table, uint32_t *fields) {
  // creating a line
  Row *row = (Row *)malloc(sizeof(Row));

  // copying fields to a row
  uint32_t fields_count = table->fields_count;
  row->fields = (uint32_t *)calloc(fields_count, sizeof(uint32_t));
  for (uint32_t i = 0; i < fields_count; ++i) row->fields[i] = fields[i];

  // increasing the table by one row
  table->size++;
  table->rows = (Row **)realloc(table->rows, table->size * sizeof(Row *));
  table->rows[table->size - 1] = row;
}

void RowDestroy(Row *row) {
  free(row->fields);
  free(row);
}

void TableDestroy(Table *table) {
  uint32_t table_size = table->size;
  for (uint32_t i = 0; i < table_size; ++i)
    if (table->rows[i]) RowDestroy(table->rows[i]);
  free(table->rows);
  free(table);
}