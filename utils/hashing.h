#include <stdlib.h>
#include <stdbool.h>

#ifndef HASHING_H
#define HASHING_H

typedef size_t hash_value_t;

typedef hash_value_t (*hash_func_t)(void const*);
typedef bool (*eq_func_t)(void const*, void const*);

#endif