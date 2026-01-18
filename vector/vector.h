#include <stdlib.h>

#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
  size_t size;
  size_t capacity;
  void* start;
  size_t item_size;
} vector;

// Create a new vector.
vector* vector_init(const size_t item_size);

// Create a new vector and reserve enough memory to fit the given capacity. Behaves like vector_init if capacity is 0.
vector* vector_init_with_capacity(const size_t item_size, const size_t capacity);

// Appends a new item to the vector, make sure to only add items of the same
// size as the initial type size.
void vector_append(vector* const v, const void* new_item);

// Returns a pointer to the item with the given index. This does not make a
// copy. This can cause dangeling pointers, use with caution. Returns NULL if
// the index is out of bounds.
void* vector_get(vector* const v, const size_t index);

// Returns a pointer to a copy of the item with the given index. Returns NULL if
// the index is out of bounds.
void* vector_get_copy(vector* const v, const size_t index);

// Pops the last element.
void vector_pop(vector* const v);

// Removes the element with the given index. Does nothing if index is invalid.
void vector_remove(vector* const v, const size_t index);

// Clear the vector
void vector_clear(vector* const v);

// Inserts the new item before index.
void vector_insert(vector* const v, const size_t index, const void* new_item);

// Inserts the new item before index.
void vector_free(vector* const v);

#endif