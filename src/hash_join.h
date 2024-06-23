#ifndef SRC_HASH_JOIN_H
#define SRC_HASH_JOIN_H

#include "DynamicArray/dynamic_array.h"
#include "HashTable/hash_table.h"
#include "Table/table.h"

Table *HashJoinImpl(Table *t1, Table *t2);

#endif  // SRC_HASH_JOIN_H