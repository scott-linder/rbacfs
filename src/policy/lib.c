#include "lib.h"
#include "../parse/lib.h"
#include "../hashmap/lib.h"
#include "../list/lib.h"

/*
 * perms_make
 *      IN: A pointer to an object definition.
 *      OUT: A perms bitmask reflecting the permslist of the given object definition.
 *      DESCRIPTION: Condenses the linked list permission format obtained from
 *      the parser into a bitfield representation for fast lookup.
 */
perms *perms_make(struct def *def) {
    perms *p = malloc(sizeof(perms));
    *p = 0;

    if (def->obj.recursive)
        *p |= PERM_RECURSIVE;

    for (struct list *permslist = def->obj.perms; permslist; permslist = list_next(permslist)) {
        switch (list_string(permslist)[0]) {
        case 'r':
            *p |= PERM_READ;
            break;
        case 'w':
            *p |= PERM_WRITE;
            break;
        case 'x':
            *p |= PERM_EXECUTE;
            break;
        }
    }

    return p;
}

/*
 * policy_build
 *      IN: A list of definitions.
 *      OUT: A policy object.
 *      DESCRIPTION: The policy object provides efficient lookups of:
 *              * Roles given a user, via policy.user_role[user]
 *              * Perms given an object, via policy.obj_role_perms[obj][role]
 */
struct policy policy_build(struct def *def) {
    struct policy policy;
    perms *p;

    policy.user_role = hashmap_create();
    policy.obj_role_perms = hashmap_create();

    for (; def; def = def->next) {
        switch (def->type) {
        case DEF_USER:
            for (struct list *users = def->user.users; users; users = list_next(users)) {
                struct list *list = hashmap_get(policy.user_role, list_string(users));
                for (struct list *roles = def->roles; roles; roles = list_next(roles)) {
                    list_append(&list, list_string(roles));
                }
                hashmap_set(policy.user_role, list_string(users), list);
            }
            break;
        case DEF_OBJ:
            p = perms_make(def);
            struct hashmap *inner;
            if (!(inner = hashmap_get(policy.obj_role_perms, def->obj.obj))) {
                inner = hashmap_create();
                hashmap_set(policy.obj_role_perms, def->obj.obj, inner);
            }
            for (struct list *roles = def->roles; roles; roles = list_next(roles)) {
                hashmap_set(inner, list_string(roles), p);
            }
            break;
        }
    }

    return policy;
}
