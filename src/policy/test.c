#include <assert.h>
#include <string.h>
#include "lib.h"
#include "../parse/lib.h"
#include "../list/lib.h"

int main(void) {
    struct def *def = rbac_parse_defs("rbac.defs");
    struct policy policy = policy_build(def);

    struct list *list = hashmap_get(policy.user_role, "user1");
    assert(strcmp(list_value(list), "role1") == 0);

    struct hashmap *inner = hashmap_get(policy.obj_role_perms, "/usr");
    assert(inner);
    perms *p = hashmap_get(inner, "role1");
    assert(p);
    assert(*p == (PERM_READ | PERM_RECURSIVE));
}
