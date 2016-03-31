/*
 * Hashmap from string to void*
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lib.h"

struct hashmap_entry {
    char *key;
    void *value;
    struct hashmap_entry *next;
};

struct hashmap {
    struct hashmap_entry **buckets;
};

#define BUCKET_COUNT (7)

static uint64_t hashmap_hash_s(const char *s) {
    size_t len = strlen(s);
    uint64_t hash = len;
    for (size_t i = 0; i < len; i++)
        hash += s[i];
    return hash;
}

static struct hashmap_entry **hashmap_get_bucket(struct hashmap *hashmap, uint64_t hash) {
    size_t bucket = hash % BUCKET_COUNT;
    return &hashmap->buckets[bucket];
}

struct hashmap *hashmap_create() {
    struct hashmap *hashmap = malloc(sizeof(*hashmap));
    hashmap->buckets = calloc(BUCKET_COUNT, sizeof(*hashmap->buckets));
    return hashmap;
}

void *hashmap_set(struct hashmap *hashmap, const char *key, void *value) {
    struct hashmap_entry *new = malloc(sizeof(*new));
    uint64_t hash = hashmap_hash_s(key);
    new->key = strdup(key);
    new->value = value;
    new->next = NULL;
    struct hashmap_entry **head = hashmap_get_bucket(hashmap, hash);
    if (*head) {
        for (struct hashmap_entry *cur = *head; cur; cur = cur->next) {
            if (strcmp(cur->key, key) == 0) {
                void *ret = cur->value;
                cur->value = value;
                return ret;
            }
            if (!cur->next)
                cur->next = new;
        }
    } else {
        *head = new;
    }
    return NULL;
}

void *hashmap_get(struct hashmap *hashmap, const char *key) {
    uint64_t hash = hashmap_hash_s(key);
    struct hashmap_entry *head = *hashmap_get_bucket(hashmap, hash);
    for (; head; head = head->next) {
        if (strcmp(head->key, key) == 0)
            return head->value;
    }
    return NULL;
}
