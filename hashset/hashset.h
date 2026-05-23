#include "hashmap/hashmap.h"
#include "stdbool.h"
#include "utils/hashing.h"

#ifndef HASHSET_H
#define HASHSET_H

typedef hashmap hashset;

// Inits the hashset with the given params. You can use these templates for the
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
// Returns true on success, false on allocation failure or invalid arguments.
bool hashset_init(size_t key_size, hash_func_t hash_func, eq_func_t eq_func,
                  hashset** const out);

// Adds a new item to the hashset or does nothing if it already exists.
// Returns true on success, false on allocation failure or invalid arguments.
bool hashset_set(hashset* set, void const* key);

// Checks if an item exists in the hashset. Out is set to true if the key was
// found, false if not.
// Returns true always.
bool hashset_test(hashset* set, void const* key, bool* const out);

// Removes the given item from the hashset, does nothing if the item does not
// exist.
// Returns true always.
bool hashset_remove(hashset* set, void const* key);

// Frees the hashset.
// Returns true on success, false if the hashset pointer is invalid.
bool hashset_free(hashset const* set);

#endif
