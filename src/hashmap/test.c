#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "lib.h"

int main(void) {
    struct hashmap *hashmap = hashmap_create();

    hashmap_set(hashmap, "ab", "ab");
    hashmap_set(hashmap, "ba", "ba");
    hashmap_set(hashmap, "123", "123");
    hashmap_set(hashmap, "This Is A String With Spaces", "This Is A String With Spaces");
    hashmap_set(hashmap, "String with spaces and 1332", "String with spaces and 1332");
    hashmap_set(hashmap, "","");
    assert(strcmp(hashmap_get(hashmap, "ab"), "ab") == 0);
    assert(strcmp(hashmap_get(hashmap, "ba"), "ba") == 0);
    assert(strcmp(hashmap_get(hashmap, "123"), "123") == 0);
    assert(strcmp(hashmap_get(hashmap, "This Is A String With Spaces"), "This Is A String With Spaces") == 0);
    assert(strcmp(hashmap_get(hashmap, "String with spaces and 1332"), "String with spaces and 1332") == 0);
    assert(strcmp(hashmap_get(hashmap, ""), "") == 0);
}
