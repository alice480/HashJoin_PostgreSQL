#ifndef SRC_DYNAMICARRAY_DYNAMIC_ARRAY_H
#define SRC_DYNAMICARRAY_DYNAMIC_ARRAY_H

#include <memory.h>
#include <stdlib.h>

typedef struct DynamicArray {
    int* values;
    unsigned int size;
} DynamicArray;

DynamicArray *CreateDynamicArray();
void InsertIntoDynamicArray(DynamicArray* array, int value);
void FreeDynamicArray(DynamicArray *array);


#endif  // SRC_DYNAMICARRAY_DYNAMIC_ARRAY_H