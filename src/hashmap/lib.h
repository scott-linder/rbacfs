#ifndef LIB_HASHMAP_H
#define LIB_HASHMAP_H
#include <stdint.h>

struct hashmap;

struct hashmap *hashmap_create();
void *hashmap_set(struct hashmap *hashmap, const char *key, void *value);
void *hashmap_get(struct hashmap *hashmap, const char *key);
#endif
