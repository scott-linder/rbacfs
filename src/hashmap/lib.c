/*
 * Hashmap from string to void*
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lib.h"

struct hashmap_entry {
    uint64_t hash;
    void *value;
    struct hashmap_entry *next;
};

struct hashmap {
    struct hashmap_entry **buckets;
};

#define BUCKET_COUNT (7)

static struct hashmap_entry **hashmap_get_bucket(struct hashmap *hashmap, uint64_t hash) {
    size_t bucket = hash % BUCKET_COUNT;
    return &hashmap->buckets[bucket];
}

struct hashmap *hashmap_create() {
    struct hashmap *hashmap = malloc(sizeof(*hashmap));
    hashmap->buckets = calloc(BUCKET_COUNT, sizeof(*hashmap->buckets));
    return hashmap;
}

void hashmap_set(struct hashmap *hashmap, uint64_t hash, void *value) {
    struct hashmap_entry *new = malloc(sizeof(*new));
    new->hash = hash;
    new->value = value;
    new->next = NULL;
    struct hashmap_entry **head = hashmap_get_bucket(hashmap, hash);
    if (*head) {
        struct hashmap_entry *cur = *head;
        for (; cur->next; cur = cur->next)
            ;
        cur->next = new;
    } else {
        *head = new;
    }
}

void *hashmap_get(struct hashmap *hashmap, uint64_t hash) {
    struct hashmap_entry *head = *hashmap_get_bucket(hashmap, hash);
    for (; head; head = head->next) {
        if (head->hash == hash)
            return head->value;
    }
    return NULL;
}

uint64_t hashmap_hash_s(const char *s) {
    size_t len = strlen(s);
    uint64_t hash = len;
    for (size_t i = 0; i < len; i++)
        hash += s[i];
    return hash;
}
