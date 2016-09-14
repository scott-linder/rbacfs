/*
 * Hashmap from string to void*
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lib.h"

/*
 * One hash entry (a linked list)
 */
struct hashmap_entry {
    char *key;
    void *value;
    struct hashmap_entry *next;
};
/*
 * A collection of buckets (the hashmap)
 */
struct hashmap {
    struct hashmap_entry **buckets;
};

#define BUCKET_COUNT (7)
/*
 *  hashmap_hash_s (The hashing function)
 *      IN: Value to be hashed
 *      OUT: returns the hash value
 *      DESCRIPTION: Creates a hash for the key value
 */
static uint64_t hashmap_hash_s(const char *s) {
    size_t len = strlen(s);
    uint64_t hash = len;

    for (size_t i = 0; i < len; i++)
        hash += s[i];
    return hash;
}
/*
 *  hashmap_entry
 *      IN: Hashmap, the hashed value
 *      OUT: A bucket entry in the hashmap
 *      DESCRIPTION: Helps enter a new hash into the map, each value enters its designated bucket
 */
static struct hashmap_entry **hashmap_get_bucket(struct hashmap *hashmap, uint64_t hash) {
    size_t bucket = hash % BUCKET_COUNT;
    return &hashmap->buckets[bucket];
}
/*
 *  hashmap_create
 *      IN: None
 *      OUT: a empty hashmap
 *      DESCRIPTION: Opens up memory space for the hashmap
 */
struct hashmap *hashmap_create() {
    struct hashmap *hashmap = malloc(sizeof(*hashmap));
    hashmap->buckets = calloc(BUCKET_COUNT, sizeof(*hashmap->buckets));
    return hashmap;
}
/*
 * hashmap_set
 *     IN: the hashmap, new key, and value
 *     OUT:
 *     DESCRIPTION: Sets a new value in the hashmap
 */
void *hashmap_set(struct hashmap *hashmap, const char *key, void *value) {
    struct hashmap_entry *new = malloc(sizeof(*new));
    /* Hash the value */
    uint64_t hash = hashmap_hash_s(key);
    /* New value to be put into the list */
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
/*  hashmap_get
 *       IN: A hashmap, key value to get
 *       OUT: Returns a void* of the value if found, else nothing
 *       DESCRIPTION: Linearly search through a bucket for a value
 */
void *hashmap_get(struct hashmap *hashmap, const char *key) {
    /* Hash the value */
    uint64_t hash = hashmap_hash_s(key);
    /*Look in the appropriate bucket for the value*/
    struct hashmap_entry *head = *hashmap_get_bucket(hashmap, hash);
    for (; head; head = head->next) {
        if (strcmp(head->key, key) == 0)
            return head->value;
    }
    return NULL;
}
