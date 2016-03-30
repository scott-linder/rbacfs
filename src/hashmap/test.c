#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "lib.h"

int main(void) {
    struct hashmap *hashmap = hashmap_create();
    hashmap_set(hashmap, 0, (void *) 1);
    assert(hashmap_get(hashmap, 0) == (void *) 1);
    hashmap_set(hashmap, 7, (void *) 2);
    assert(hashmap_get(hashmap, 7) == (void *) 2);
    hashmap_set(hashmap, hashmap_hash_s("test"), (void *) 3);
    assert(hashmap_get(hashmap, hashmap_hash_s("test")) == (void *) 3);
    assert(hashmap_get(hashmap, hashmap_hash_s("not there")) == (void *) 0);
}
