#ifndef LIB_POLICY_H
#define LIB_POLICY_H
#include <stdint.h>
#include "../parse/lib.h"
#include "../hashmap/lib.h"
struct policy {
    /*
     * user_role[user] = role;
     */
    struct hashmap *user_role;
    /*
     * obj_role_perms[obj][role] = perms;
     */
    struct hashmap *obj_role_perms;
};

typedef uint8_t perms;

enum perms {
    PERM_READ = 1,
    PERM_WRITE = 1 << 1,
    PERM_EXECUTE = 1 << 2,
    PERM_RECURSIVE = 1 << 3,
};

struct policy policy_build(struct def *def);
#endif
