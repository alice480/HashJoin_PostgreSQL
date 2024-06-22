#include "table.h"

Table* CreateTable(int rows) {
    Table *table = (Table *)malloc(sizeof(Table));
    table->size = rows;
    table->rows = (Row **)calloc(rows, sizeof(Row *));
    return table;
}

void InsertIntoRow(Table *table, unsigned int row_number, int a, int b) {
    Row *row = (Row *)malloc(sizeof(Row));
    row->a = a;
    row->b = b;
    table->rows[row_number] = row;
}

void FreeRow(Row *row) {
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