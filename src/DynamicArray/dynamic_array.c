#include "dynamic_array.h"

DynamicArray *DynamicArrayCreate() {
  DynamicArray *array = (DynamicArray *)malloc(sizeof(DynamicArray));
  array->values = (int *)malloc(sizeof(int));
  array->size = 0;
  return array;
}

void DynamicArrayInsert(DynamicArray *array, int value) {
  array->values =
      (int *)realloc(array->values, (array->size + 1) * sizeof(int));
  array->values[array->size] = value;
  array->size++;
}

void DynamicArrayDestroy(DynamicArray *array) {
  free(array->values);
  free(array);
}
