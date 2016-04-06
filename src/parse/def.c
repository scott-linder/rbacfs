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
    slist->next = malloc(sizeof(*slist->next));
    slist->next->s = s;
    slist->next->next = NULL;
}

void slist_destroy(struct slist *slist) {
    struct slist *next;
    for (; slist; slist = next) {
        next = slist->next;
        free(slist->s);
        free(slist);
    }
}

struct def *def_create(enum def_type type) {
    struct def *def = malloc(sizeof(*def));
    def->type = type;
    def->next = NULL;
    return def;
}

void def_append(struct def *list, struct def *def) {
    for (; list->next; list = list->next)
        ;
    list->next = def;
}

void def_destroy(struct def *list) {
    struct def *next;
    for (; list; list = next) {
        next = list->next;
        slist_destroy(list->roles);
        switch (list->type) {
        case DEF_USER:
            free(list->user.users);
            break;
        case DEF_OBJ:
            free(list->obj.obj);
            slist_destroy(list->obj.perms);
            break;
        }
        free(list);
    }
}
