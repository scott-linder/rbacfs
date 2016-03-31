#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "lib.h"

int main(void) {
    struct hashmap *hashmap = hashmap_create();

    hashmap_set(hashmap, "ab", "ab");
    hashmap_set(hashmap, "ba", "ba");
    assert(strcmp(hashmap_get(hashmap, "ab"), "ab") == 0);
    assert(strcmp(hashmap_get(hashmap, "ba"), "ba") == 0);
}
