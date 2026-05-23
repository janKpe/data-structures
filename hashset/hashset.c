#include "hashset.h"
#include "hashmap/hashmap.h"
#include "utils/hashing.h"
#include "utils/utils.h"
#include <stdbool.h>

bool hashset_init(size_t key_size, hash_func_t hash_func, eq_func_t eq_func,
                  hashset** const out) {
  return hashmap_init(key_size, sizeof(bool), hash_func, eq_func, out);
}

bool hashset_set(hashset* set, void const* key) {
  bool dummy_val = true;
  return hashmap_put(set, key, &dummy_val);
}

bool hashset_test(hashset* set, void const* key, bool* const out) {
  bool* dummy_val_p;
  if (hashmap_get(set, key, (void**)&dummy_val_p)) {
    *out = dummy_val_p != NULL;
  } else {
    *out = false;
  }
  return true;
}

bool hashset_remove(hashset* set, void const* key) {
  hashmap_remove(set, key);
  return true;
}

bool hashset_free(hashset const* set) { return hashmap_free((hashmap*)set); }
