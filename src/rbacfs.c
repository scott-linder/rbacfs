#include <stdlib.h>
#include <stdio.h>
#include "parse/lib.h"
#include "fuse/lib.h"

int main(int argc, char *argv[]) {
    struct def *defs;

    if (argc == 4) {
        defs = rbac_parse_defs(argv[--argc]);
        argv[argc] = NULL;
    } else {
        fprintf(stderr, "usage: %s mount root rbac.defs\n", argv[0]);
        exit(1);
    }

    fuse_start(argc, argv, defs);
}
