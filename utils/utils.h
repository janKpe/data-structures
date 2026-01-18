#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H

#define RETURN_NULL_IF_NULL(...)                                               \
  do {                                                                         \
    const void* const _args[] = {__VA_ARGS__};                                 \
    for (unsigned i = 0; i < sizeof(_args) / sizeof(_args[0]); i++) {          \
      if (_args[i] == NULL) {                                                  \
        return NULL;                                                           \
      }                                                                        \
    }                                                                          \
  } while (0)

#define RETURN_FALSE_IF_NULL(...)                                              \
  do {                                                                         \
    const void* const _args[] = {__VA_ARGS__};                                 \
    for (unsigned i = 0; i < sizeof(_args) / sizeof(_args[0]); i++) {          \
      if (_args[i] == NULL) {                                                  \
        return false;                                                          \
      }                                                                        \
    }                                                                          \
  } while (0)

#define RETURN_IF_NULL(...)                                                    \
  do {                                                                         \
    const void* const _args[] = {__VA_ARGS__};                                 \
    for (unsigned i = 0; i < sizeof(_args) / sizeof(_args[0]); i++) {          \
      if (_args[i] == NULL) {                                                  \
        return;                                                                \
      }                                                                        \
    }                                                                          \
  } while (0)

#endif
