#include "table.h"

Table* CreateTable(int fields) {
    Table *table = (Table *)malloc(sizeof(Table));
    table->rows = (Row **)malloc(sizeof(Row *));
    table->size = 0;
    table->fields_count = fields;
    return table;
}

void InsertIntoTable(Table *table, int *fields) {
    // creating a line
    Row *row = (Row *)malloc(sizeof(Row));
 
    // copying fields to a string
    unsigned int fields_count = table->fields_count;
    row->fields = (int *)calloc(fields_count, sizeof(int));
    for (unsigned int i = 0; i < fields_count; ++i)
        row->fields[i] = fields[i];

    // increasing the table by one row
    table->size++;
    table->rows = (Row **)realloc(table->rows, table->size * sizeof(Row *));
    table->rows[table->size - 1] = row;
}

void FreeRow(Row *row) {
    free(row->fields);
    free(row);
}

void FreeTable(Table *table) {
    unsigned int table_size = table->size; 
    for (unsigned int i = 0; i < table_size; ++i)
        if (table->rows[i])
            FreeRow(table->rows[i]);
    free(table->rows);
    free(table);
}