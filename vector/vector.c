#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void vector_increase_size(vector* const v);

vector* vector_init(const size_t item_size) {
  vector* v = malloc(sizeof(vector));

  if (v == NULL) {
    return NULL;
  }
  v->capacity = 0;
  v->size = 0;
  v->start = NULL;
  v->item_size = item_size;
  return v;
}

// Increases the vectors size by one and reasizes it if necessary. Make sure to
// pass in a valid vector, it is not validated.
static void vector_increase_size(vector* const v) {
  size_t new_size = v->size + 1;
  if (new_size > v->capacity) {
    int new_capacity;
    if (v->capacity == 0) {
      new_capacity = 1;
    } else {
      new_capacity = v->capacity * 2;
    }
    void* new_start = realloc(v->start, new_capacity * v->item_size);
    if (!new_start) {
      return;
    }

    v->capacity = new_capacity;
    v->start = new_start;
  }
  v->size = new_size;
}

// Appends a new item to the vector, make sure to only add items of the same
// size as the initial type size. Make sure to pass in a valid vector, it is not
// validated.
void vector_append(vector* const v, const void* new_item) {
  int old_size = v->size;
  vector_increase_size(v);
  memmove(v->start + (old_size * v->item_size), new_item, v->item_size);
}

// Returns a pointer to a copy of the item with the given index. Returns NULL if
// the index is out of bounds.  Make sure to pass in a valid vector, it is not
// validated.
void* vector_get(vector* const v, const size_t index) {
  if (index >= v->size) {
    return NULL;
  }

  int* item_copy = malloc(v->item_size);
  if (!item_copy) {
    return NULL;
  }
  memmove(item_copy, v->start + (index * v->item_size), v->item_size);
  return item_copy;
}

// Pops the last element. Make sure to pass in a valid vector, it is not
// validated.
void vector_pop(vector* const v) {
  if (v->size <= 0) {
    return;
  }
  v->size -= 1;
}

// Removes the element with the given index. Does nothing if index is invalid.
// Make sure to pass in a valid vector, it is not validated.
void vector_remove(vector* const v, const size_t index) {
  if (index < 0 || index >= v->size) {
    return;
  }

  char* base = (char*)v->start;
  char* src = base + index * v->item_size;
  char* dest = base + (index + 1) * v->item_size;
  // decrease the size here because we want to move one item less then we
  // currently have because we are currently removing one.
  v->size -= 1;
  size_t len = (v->size - index) * v->item_size;
  memmove(src, dest, len);
}

// Make sure to pass in a valid vector, it is not validated.
void vector_clear(vector* const v) { v->size = 0; }

// Inserts the new item before index.
// Make sure to pass in a valid vector, it is not validated.
void vector_insert(vector* const v, size_t index, const void* new_item) {
  int old_size = v->size;
  vector_increase_size(v);

  char* base = (char*)v->start;

  memmove(base + (index + 1) * v->item_size, base + index * v->item_size,
          (old_size - index) * v->item_size);

  memcpy(base + index * v->item_size, new_item, v->item_size);
}

// Frees the vector and sets the start pointer to NULL.
// Make sure to pass in a valid vector, it is not validated.
void vector_free(vector* const v) {
  free(v->start);
  free(v);
}
