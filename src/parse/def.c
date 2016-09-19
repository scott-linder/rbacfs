/*
 * def.c - Syntax tree.
 */

#include <stdlib.h>
#include "def.h"
#include "../list/lib.h"

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
        list_destroy(list->roles);
        switch (list->type) {
        case DEF_USER:
            free(list->user.users);
            break;
        case DEF_OBJ:
            free(list->obj.obj);
            list_destroy(list->obj.perms);
            break;
        }
        free(list);
    }
}
