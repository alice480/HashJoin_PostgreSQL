#ifndef SRC_DYNAMICARRAY_DYNAMIC_ARRAY_H
#define SRC_DYNAMICARRAY_DYNAMIC_ARRAY_H

#include <memory.h>
#include <stdlib.h>

typedef struct DynamicArray {
    int* values;
    unsigned int size;
} DynamicArray;

DynamicArray *DynamicArrayCreate();
void DynamicArrayInsert(DynamicArray* array, int value);
void DynamicArrayDestroy(DynamicArray *array);


#endif  // SRC_DYNAMICARRAY_DYNAMIC_ARRAY_H