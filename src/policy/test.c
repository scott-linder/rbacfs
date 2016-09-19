#include <assert.h>
#include <string.h>
#include "lib.h"
#include "../parse/lib.h"
#include "../list/lib.h"

int main(void) {
    struct def *def = rbac_parse_defs("rbac.defs");
    struct policy policy = policy_build(def);
    struct list *user_role;
    struct hashmap *role;
    perms *p;

    /* user: role1 user1 */
    user_role = hashmap_get(policy.user_role, "user1");
    assert(user_role);
    assert(strcmp(list_value(user_role), "role1") == 0);
    assert(!list_next(user_role));
    /* user: role1,role2,role3 user2 */
    user_role = hashmap_get(policy.user_role, "user2");
    assert(user_role);
    assert(strcmp(list_value(user_role), "role1") == 0);
    assert(user_role = list_next(user_role));
    assert(strcmp(list_value(user_role), "role2") == 0);
    assert(user_role = list_next(user_role));
    assert(strcmp(list_value(user_role), "role3") == 0);
    assert(!list_next(user_role));
    /* user: role2 user3,user4 */
    user_role = hashmap_get(policy.user_role, "user3");
    assert(user_role);
    assert(strcmp(list_value(user_role), "role2") == 0);
    assert(!list_next(user_role));
    user_role = hashmap_get(policy.user_role, "user4");
    assert(user_role);
    assert(strcmp(list_value(user_role), "role2") == 0);
    assert(!list_next(user_role));
    /* user: role1,role2 user5,user6 */
    user_role = hashmap_get(policy.user_role, "user5");
    assert(user_role);
    assert(strcmp(list_value(user_role), "role1") == 0);
    assert(user_role = list_next(user_role));
    assert(strcmp(list_value(user_role), "role2") == 0);
    assert(!list_next(user_role));
    user_role = hashmap_get(policy.user_role, "user6");
    assert(user_role);
    assert(strcmp(list_value(user_role), "role1") == 0);
    assert(user_role = list_next(user_role));
    assert(strcmp(list_value(user_role), "role2") == 0);
    assert(!list_next(user_role));
    /* unknown user */
    user_role = hashmap_get(policy.user_role, "user7");
    assert(!user_role);

    /* object: role1 r -r /usr */
    role = hashmap_get(policy.obj_role_perms, "/usr");
    assert(role);
    p = hashmap_get(role, "role1");
    assert(p);
    assert(*p == (PERM_READ | PERM_RECURSIVE));
    p = hashmap_get(role, "role2");
    assert(!p);
    /* object: role2 r,x -r /lib */
    role = hashmap_get(policy.obj_role_perms, "/lib");
    assert(role);
    p = hashmap_get(role, "role2");
    assert(p);
    assert(*p == (PERM_READ | PERM_EXECUTE | PERM_RECURSIVE));
    /* object: role2,role3 r,w,x /usr/file1.txt */
    role = hashmap_get(policy.obj_role_perms, "/usr/file1.txt");
    assert(role);
    p = hashmap_get(role, "role2");
    assert(p);
    assert(*p == (PERM_READ | PERM_WRITE | PERM_EXECUTE));
    p = hashmap_get(role, "role3");
    assert(p);
    assert(*p == (PERM_READ | PERM_WRITE | PERM_EXECUTE));
    /* object: role1 x /file3.txt */
    role = hashmap_get(policy.obj_role_perms, "/file3.txt");
    assert(role);
    p = hashmap_get(role, "role1");
    assert(p);
    assert(*p == (PERM_EXECUTE));
    /* object: role1 x /file2.txt */
    role = hashmap_get(policy.obj_role_perms, "/file2.txt");
    assert(role);
    p = hashmap_get(role, "role1");
    assert(p);
    assert(*p == (PERM_EXECUTE));
}
