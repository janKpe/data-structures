#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"

static void vector_increase_size(vector* const v);

vector* vector_init(const size_t item_size) {
  vector* v = malloc(sizeof(vector));

  RETURN_NULL_IF_NULL(v);

  v->capacity = 0;
  v->size = 0;
  v->start = NULL;
  v->item_size = item_size;
  return v;
}

vector* vector_init_with_capacity(const size_t item_size, const size_t capacity) {
  vector* v = malloc(sizeof(vector));

  RETURN_NULL_IF_NULL(v);

  v->capacity = capacity;
  v->size = 0;
  v->item_size = item_size;

  if (capacity == 0) {
    v->start = NULL;
  } else {
    v->start = malloc(capacity * v->item_size);
    if (v->start == NULL) {
      return NULL;
    }
  }

  return v;
}

// Increases the vectors size by one and reasizes it if necessary.
static void vector_increase_size(vector* const v) {
  RETURN_IF_NULL(v);

  size_t new_size = v->size + 1;
  if (new_size > v->capacity) {
    int new_capacity;
    if (v->capacity == 0) {
      new_capacity = 1;
    } else {
      new_capacity = v->capacity * 2;
    }
    void* new_start = realloc(v->start, new_capacity * v->item_size);
    RETURN_IF_NULL(new_start);

    v->capacity = new_capacity;
    v->start = new_start;
  }
  v->size = new_size;
}

void vector_append(vector* const v, const void* new_item) {
  RETURN_IF_NULL(v);
  RETURN_IF_NULL(new_item);
  int old_size = v->size;
  vector_increase_size(v);
  memmove(v->start + (old_size * v->item_size), new_item, v->item_size);
}

void* vector_get_copy(vector* const v, const size_t index) {
  RETURN_NULL_IF_NULL(v);
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

void* vector_get(vector* const v, const size_t index) {
  RETURN_NULL_IF_NULL(v);
  if (index >= v->size) {
    return NULL;
  }

  return v->start + (index * v->item_size);
}

void vector_pop(vector* const v) {
  RETURN_IF_NULL(v);

  if (v->size <= 0) {
    return;
  }
  v->size -= 1;
}

void vector_remove(vector* const v, const size_t index) {
  RETURN_IF_NULL(v);

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

void vector_clear(vector* const v) {
  RETURN_IF_NULL(v);
  v->size = 0;
}

void vector_insert(vector* const v, size_t index, const void* new_item) {
  RETURN_IF_NULL(v);
  RETURN_IF_NULL(new_item);

  int old_size = v->size;
  vector_increase_size(v);

  char* base = (char*)v->start;

  memmove(base + (index + 1) * v->item_size, base + index * v->item_size,
          (old_size - index) * v->item_size);

  memcpy(base + index * v->item_size, new_item, v->item_size);
}

void vector_free(vector* const v) {
  RETURN_IF_NULL(v);
  free(v->start);
  free(v);
}
