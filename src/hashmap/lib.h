#ifndef LIB_HASHMAP_H
#define LIB_HASHMAP_H
#include <stdint.h>

struct hashmap;

struct hashmap *hashmap_create();
void hashmap_set(struct hashmap *hashmap, uint64_t hash, void *value);
void *hashmap_get(struct hashmap *hashmap, uint64_t hash);
uint64_t hashmap_hash_s(const char *s);
#endif
