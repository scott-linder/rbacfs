/*
 * def.c - Syntax tree.
 */

#include <stdlib.h>
#include "def.h"

struct slist *slist_create(void) {
    return malloc(sizeof(struct slist));
}

void slist_append(struct slist *slist, char *s) {
    for (; slist->next; slist = slist->next)
        ;
    slist->next = malloc(sizeof(slist->next));
    slist->next->s = s;
    slist->next->next = NULL;
}

void slist_destroy(struct slist *slist) {
    struct slist *next;
    for (; slist; slist = next) {
        next = slist->next;
        free(slist);
    }
}

struct def *def_create(enum def_type type) {
    struct def *def = malloc(sizeof(*def));
    def->type = type;
    return def;
}
