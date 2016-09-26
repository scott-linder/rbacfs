#include <assert.h>
#include <string.h>
#include "lib.h"
#include "../list/lib.h"

int main(void) {
    struct def *def = rbac_parse_defs("rbac.defs");
    struct list *roles, *users, *perms;

    /* user: role1 user1 */
    assert(def->type == DEF_USER);
    assert(roles = def->roles);
    assert(strcmp(list_string(roles), "role1") == 0);
    assert(!list_next(roles));
    assert(users = def->user.users);
    assert(strcmp(list_string(users), "user1") == 0);
    assert(!list_next(users));

    /* user: role1,role2,role3 user2 */
    assert(def = def->next);
    assert(def->type == DEF_USER);
    assert(roles = def->roles);
    assert(strcmp(list_string(roles), "role1") == 0);
    assert(roles = list_next(roles));
    assert(strcmp(list_string(roles), "role2") == 0);
    assert(roles = list_next(roles));
    assert(strcmp(list_string(roles), "role3") == 0);
    assert(!list_next(roles));
    assert(users = def->user.users);
    assert(strcmp(list_string(users), "user2") == 0);
    assert(!list_next(users));

    /* user: role2 user3,user4 */
    assert(def = def->next);
    assert(def->type == DEF_USER);
    assert(roles = def->roles);
    assert(strcmp(list_string(roles), "role2") == 0);
    assert(!list_next(roles));
    assert(users = def->user.users);
    assert(strcmp(list_string(users), "user3") == 0);
    assert(users = list_next(users));
    assert(strcmp(list_string(users), "user4") == 0);
    assert(!list_next(users));

    /* user: role1,role2 user5,user6 */
    assert(def = def->next);
    assert(def->type == DEF_USER);
    assert(roles = def->roles);
    assert(strcmp(list_string(roles), "role1") == 0);
    assert(roles = list_next(roles));
    assert(strcmp(list_string(roles), "role2") == 0);
    assert(!list_next(roles));
    assert(users = def->user.users);
    assert(strcmp(list_string(users), "user5") == 0);
    assert(users = list_next(users));
    assert(strcmp(list_string(users), "user6") == 0);
    assert(!list_next(users));

    /* object: role1 r -r /usr */
    assert(def = def->next);
    assert(def->type == DEF_OBJ);
    assert(roles = def->roles);
    assert(strcmp(list_string(roles), "role1") == 0);
    assert(!list_next(roles));
    assert(strcmp(def->obj.obj, "/usr") == 0);
    assert(perms = def->obj.perms);
    assert(strcmp(list_string(perms), "r") == 0);
    assert(!list_next(perms));
    assert(def->obj.recursive);

    /* object: role2 r,x -r /lib */
    assert(def = def->next);
    assert(def->type == DEF_OBJ);
    assert(roles = def->roles);
    assert(strcmp(list_string(roles), "role2") == 0);
    assert(!list_next(roles));
    assert(strcmp(def->obj.obj, "/lib") == 0);
    assert(perms = def->obj.perms);
    assert(strcmp(list_string(perms), "r") == 0);
    assert(perms = list_next(perms));
    assert(strcmp(list_string(perms), "x") == 0);
    assert(!list_next(perms));
    assert(def->obj.recursive);

    /* object: role2,role3 r,w,x /usr/file1.txt */
    assert(def = def->next);
    assert(def->type == DEF_OBJ);
    assert(roles = def->roles);
    assert(strcmp(list_string(roles), "role2") == 0);
    assert(roles = list_next(roles));
    assert(strcmp(list_string(roles), "role3") == 0);
    assert(!list_next(roles));
    assert(strcmp(def->obj.obj, "/usr/file1.txt") == 0);
    assert(perms = def->obj.perms);
    assert(strcmp(list_string(perms), "r") == 0);
    assert(perms = list_next(perms));
    assert(strcmp(list_string(perms), "w") == 0);
    assert(perms = list_next(perms));
    assert(strcmp(list_string(perms), "x") == 0);
    assert(!list_next(perms));
    assert(!def->obj.recursive);

    /* object: role1 x /file3.txt */
    assert(def = def->next);
    assert(def->type == DEF_OBJ);
    assert(roles = def->roles);
    assert(strcmp(list_string(roles), "role1") == 0);
    assert(!list_next(roles));
    assert(strcmp(def->obj.obj, "/file3.txt") == 0);
    assert(perms = def->obj.perms);
    assert(strcmp(list_string(perms), "x") == 0);
    assert(!list_next(perms));
    assert(!def->obj.recursive);

    /* object: role1 x /file3.txt */
    assert(def = def->next);
    assert(def->type == DEF_OBJ);
    assert(roles = def->roles);
    assert(strcmp(list_string(roles), "role1") == 0);
    assert(!list_next(roles));
    assert(strcmp(def->obj.obj, "/file2.txt") == 0);
    assert(perms = def->obj.perms);
    assert(strcmp(list_string(perms), "x") == 0);
    assert(!list_next(perms));
    assert(!def->obj.recursive);
}
