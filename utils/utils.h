
#ifndef UTILS_H
#define UTILS_H

#define RETURN_NULL_IF_NULL(ptr)                                               \
  do {                                                                         \
    if (ptr == NULL) {                                                         \
      return NULL;                                                             \
    }                                                                          \
  } while (0)

#define RETURN_IF_NULL(ptr)                                                    \
  do {                                                                         \
    if (ptr == NULL) {                                                         \
      return;                                                             \
    }                                                                          \
  } while (0)

#endif