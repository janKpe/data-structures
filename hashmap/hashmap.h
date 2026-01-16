#include "stdbool.h"
#include "vector/vector.h"

#ifndef HASHMAP_H
#define HASHMAP_H

#define HASHMAP_INIT_BUCKET_COUNT 12
#define HASHMAP_MAX_LOAD_FACTOR 0.75

typedef size_t hash_value_t;

typedef hash_value_t (*hash_func_t)(void const*);
typedef bool (*eq_func_t)(void const*, void const*);

typedef enum {
  RESIZE_SUCCESS = 0,
  RESIZE_NOT_NEEDED = 1,
  RESIZE_FAILED = -1
} resize_result_t;

typedef struct {
  void* key;
  void* value;
} map_entry;

typedef struct {
  size_t item_size;
  size_t key_size;
  hash_func_t hash_func;
  eq_func_t eq_func;
  size_t size; // number of entries
  vector* buckets;
} hashmap;

// Inits the hashmap with the given params. You can use these templates for the
// hash and equal functions:
//
//  Hash:
// NOTE: If the key type is just a int/enum type you can just cast it it
// hash_value_T
// ```c
// hash_value_t my_type_hash(void const* ptr) {
//   if (!ptr) {
//     return 0;
//   }
//
//   const my_type* my_val = (const my_type*)ptr;
//   hash_value_t h = 14695981039346656037ULL;
//   const hash_value_t prime = 1099511628211ULL; // FNV prime
//
//   h ^= (hash_value_t)my_val->x;
//   h *= prime;
//   h ^= (hash_value_t)my_val->y;
//   h *= prime;
//
//   return h;
// }
// ```
//
//
// Equal:
// ```c
// bool my_type_eq(void const* p1, void const* p2) {
//   if (!p1 || !p2) {
//     return false;
//   }
//
//   const my_type* a = (const my_type*)p1;
//   const my_type* b = (const my_type*)p2;
//
//   return a->x == b->x && a->y == b->y;
// }
// ```
bool hashmap_init(size_t key_size, size_t item_size, hash_func_t hash_func,
                  eq_func_t eq_func, hashmap** const out);

// Adds a new item to the hashmap or updates the value if the key already
// exists. Resizes if necessary.
bool hashmap_put(hashmap* map, void const* key, void const* new_item);

// Returns a pointer to the item with the given key. Does not make a copy.
// Returns NULL if no item is found.
bool hashmap_get(hashmap const* map, void const* key, void** const out);

// Returns a pointer to the item with the given key. Makes a copy. Caller is
// responsible for freeing allocated memory. Returns NULL if no item is found.
bool hashmap_get_copy(hashmap const* map, void const* key, void* const out);

// Removes the item with the given key from the hashmap, does nothing if the
// item does not exist.
bool hashmap_remove(hashmap* map, void const* key);

// Free the hashmap.
bool hashmap_free(hashmap* const hashmap);

#endif
