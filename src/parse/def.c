/*
 * def.c - Syntax tree.
 */

#include <stdlib.h>
#include "def.h"
#include "../list/lib.h"

/*
 *  def_create
 *      IN: type of definition (defined in def.h)
 *      OUT: definitions list
 *      DESCRIPTION: creates new definitions list as defined in def.h
 */
struct def *def_create(enum def_type type) {
    struct def *def = malloc(sizeof(*def));
    def->type = type;
    def->next = NULL;
    return def;
}

/*
 *  def_append
 *      IN: current definitions list, new definition to add
 *      OUT: N/A
 *      DESCRIPTION: Adds new definition to existing list
 */
void def_append(struct def *list, struct def *def) {
    for (; list->next; list = list->next)
        ;
    list->next = def;
}

/*
 *  def_destroy
 *      IN: definitions list
 *      OUT: N/A
 *      DESCRIPTION: Deletes/destroys the definitions list and frees that memory
 */
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
