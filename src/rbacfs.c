#include <stdlib.h>
#include <stdio.h>
#include "parse/lib.h"
#include "fuse/lib.h"
#include "policy/lib.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s mount root rbac.defs\n", argv[0]);
        exit(1);
    }

    struct def *defs = rbac_parse_defs(argv[--argc]);
    argv[argc] = NULL;

    struct policy policy = policy_build(defs);

    fuse_start(argc, argv, policy);
}
