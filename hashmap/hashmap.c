#include "hashmap.h"
#include "stdlib.h"
#include "string.h"
#include "utils/utils.h"

static resize_result_t resize_if_needed(hashmap* map);

// Resizes the hasmap if size + 1 exceedes the threashold. Does not increase
// the size (number of entries). Should be called before a new item is
// added.
static resize_result_t resize_if_needed(hashmap* map) {
  if (!map) {
    return RESIZE_FAILED;
  }

  float load = (float)(map->size + 1) / (float)map->buckets->size;
  if (load < HASHMAP_MAX_LOAD_FACTOR) {
    return RESIZE_NOT_NEEDED;
  }
  size_t new_bucket_size = map->buckets->size * 2;
  vector* new_buckets =
      vector_init_with_capacity(sizeof(vector*), new_bucket_size);
  if (!new_buckets) {
    return RESIZE_FAILED;
  }

  for (size_t i = 0; i < new_bucket_size; i++) {
    vector* bucket = vector_init_with_capacity(sizeof(map_entry), 1);
    if (!bucket) {
      // free everything when allocing more memory failed
      for (size_t j = 0; j < i; j++) {
        vector** bucket_ptr = vector_get(new_buckets, j);
        vector* bucket = *bucket_ptr;
        vector_free(bucket);
      }

      vector_free(new_buckets);
      return RESIZE_FAILED;
    }
    vector_append(new_buckets, &bucket);
  }

  // rehash every item from the old buckets and copy it to the new
  for (size_t i = 0; i < map->buckets->size; i++) {
    vector** current_bucket_ptr = vector_get(map->buckets, i);
    vector* current_bucket = *current_bucket_ptr;
    for (size_t j = 0; j < current_bucket->size; j++) {
      map_entry* entry = vector_get(current_bucket, j);
      hash_value_t hash = map->hash_func(entry->key);
      size_t index = hash % new_buckets->size;

      vector** bucket_ptr = vector_get(new_buckets, index);
      vector* bucket = *bucket_ptr;
      vector_append(bucket, entry);
      // the entry must not be freed here. It lives in current_bucket->start and
      // is freed bellow.
    }
    // free this old bucket after the entry was added to the new.
    vector_free(current_bucket);
  }
  vector_free(map->buckets);
  map->buckets = new_buckets;
  return RESIZE_SUCCESS;
}

bool hashmap_init(size_t key_size, size_t item_size, hash_func_t hash_func,
                  eq_func_t eq_func, hashmap** const out) {
  RETURN_FALSE_IF_NULL(hash_func, eq_func, out);

  hashmap* map = malloc(sizeof(hashmap));
  if (!map) {
    return false;
  }

  map->item_size = item_size;
  map->key_size = key_size;
  map->hash_func = hash_func;
  map->eq_func = eq_func;
  map->size = 0;

  vector* buckets =
      vector_init_with_capacity(sizeof(vector*), HASHMAP_INIT_BUCKET_COUNT);
  if (!buckets) {
    free(map);
    return false;
  }

  for (size_t i = 0; i < HASHMAP_INIT_BUCKET_COUNT; i++) {
    vector* v = vector_init_with_capacity(sizeof(map_entry), 1);
    if (!v) {
      // Clean up buckets created so far
      for (size_t j = 0; j < i; j++) {
        vector** bucket_ptr = vector_get(buckets, j);
        if (bucket_ptr && *bucket_ptr) {
          vector_free(*bucket_ptr);
        }
      }
      vector_free(buckets);
      free(map);
      return false;
    }
    vector_append(buckets, &v);
  }

  map->buckets = buckets;
  *out = map;
  return true;
}

bool hashmap_put(hashmap* map, void const* key, void const* new_item) {
  RETURN_FALSE_IF_NULL(map, key, new_item);

  hash_value_t hash = map->hash_func(key);
  size_t index = hash % map->buckets->size;

  vector** bucket_ptr = vector_get(map->buckets, index);
  vector* bucket = *bucket_ptr;

  for (size_t i = 0; i < bucket->size; i++) {
    map_entry* entry = vector_get(bucket, i);
    if (map->eq_func(entry->key, key)) {
      void* item = malloc(map->item_size);
      RETURN_FALSE_IF_NULL(item);

      memmove(item, new_item, map->item_size);
      free(entry->value);
      entry->value = item;
      return true;
    }
  }
  // we are adding a new item
  resize_result_t ret = resize_if_needed(map);

  switch (ret) {
  case RESIZE_SUCCESS:
    hash = map->hash_func(key);
    index = hash % map->buckets->size;
    bucket_ptr = vector_get(map->buckets, index);
    bucket = *bucket_ptr;
    break;
  case RESIZE_NOT_NEEDED:
    break;
  case RESIZE_FAILED:
    return false;
  default:
    return false;
  }

  void* new_key = malloc(map->key_size);
  void* new_value = malloc(map->item_size);

  if (!new_key || !new_value) {
    free(new_key);
    free(new_value);
    return false;
  }

  memmove(new_key, key, map->key_size);
  memmove(new_value, new_item, map->item_size);

  map_entry new_entry = {.key = new_key, .value = new_value};

  vector_append(bucket, &new_entry);
  map->size++;
  return true;
}

bool hashmap_get(hashmap const* map, void const* key, void** const out) {
  RETURN_FALSE_IF_NULL(map, key, out);

  hash_value_t hash = map->hash_func(key);
  size_t index = hash % map->buckets->size;

  vector** bucket_ptr = vector_get(map->buckets, index);
  vector* bucket = *bucket_ptr;

  for (size_t i = 0; i < bucket->size; i++) {
    map_entry* entry = vector_get(bucket, i);
    if (map->eq_func(entry->key, key)) {
      *out = entry->value;
      return true;
    }
  }
  return false;
}

bool hashmap_remove(hashmap* map, void const* key) {
  RETURN_FALSE_IF_NULL(map, key);

  hash_value_t hash = map->hash_func(key);
  size_t index = hash % map->buckets->size;

  vector** bucket_ptr = vector_get(map->buckets, index);
  vector* bucket = *bucket_ptr;

  for (size_t i = 0; i < bucket->size; i++) {
    map_entry* entry = vector_get(bucket, i);
    if (map->eq_func(entry->key, key)) {
      free(entry->key);
      free(entry->value);
      vector_remove(bucket, i);
      map->size--;
      return true;
    }
  }
  return false;
}

bool hashmap_get_copy(hashmap const* map, void const* key, void* const out) {
  RETURN_FALSE_IF_NULL(map, key, out);

  void* retrieved = NULL;
  bool found = hashmap_get(map, key, &retrieved);

  if (!found || !retrieved) {
    return false;
  }

  memmove(out, retrieved, map->item_size);
  return true;
}

bool hashmap_free(hashmap* const map) {
  RETURN_FALSE_IF_NULL(map);

  for (size_t i = 0; i < map->buckets->size; i++) {
    vector** bucket_ptr = vector_get(map->buckets, i);
    vector* bucket = *bucket_ptr;
    for (size_t i = 0; i < bucket->size; i++) {
      map_entry* entry = vector_get(bucket, i);
      free(entry->key);
      free(entry->value);
      // dont free entry here because it points into bucket->start which is
      // freed bellow when the whole bucket is freed.
    }

    vector_free(bucket);
  }
  vector_free(map->buckets);
  free(map);
  return true;
}
