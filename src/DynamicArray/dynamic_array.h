/*-------------------------------------------------------------------------
 *
 * dynamic_array.h
 *	  prototypes for dynamic_array.c
 *
 *
 * src/DynamicArray/dynamic_array.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef SRC_DYNAMICARRAY_DYNAMIC_ARRAY_H
#define SRC_DYNAMICARRAY_DYNAMIC_ARRAY_H

#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct DynamicArray {
  int* values;
  uint32_t size;
} DynamicArray;

DynamicArray* DynamicArrayCreate();
void DynamicArrayInsert(DynamicArray* array, uint32_t value);
void DynamicArrayDestroy(DynamicArray* array);

#endif  // SRC_DYNAMICARRAY_DYNAMIC_ARRAY_H