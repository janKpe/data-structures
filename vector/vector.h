#include <stdlib.h>

#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_VALIDATE(vector)                                              \
  if (vector == NULL) {                                                       \
    return;                                                                    \
  }

#define VALIDATE_PTR(ptr) VECTOR_VALIDATE(ptr)
typedef struct {
  size_t size;
  size_t capacity;
  void* start;
  size_t item_size;
} vector;

vector* vector_init(const size_t item_size);

void vector_append(vector* const v, const void* new_item);

void* vector_get(vector* const v, const size_t index);

void* vector_get_copy(vector* const v, const size_t index);

void vector_pop(vector* const v);

void vector_remove(vector* const v, const size_t index);

void vector_clear(vector* const v);

void vector_insert(vector* const v, const size_t index, const void* new_item);

void vector_free(vector* const v);

#endif