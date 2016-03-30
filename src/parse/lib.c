/*
 * main.c
 */

#include <stdio.h>
#include "def.h"
#include "parse.h"
#include "lex.h"

int yyparse(struct def **def, yyscan_t scanner);

struct def *rbac_parse_defs(const char *defs_filename) {
    struct def *def;
    yyscan_t scanner;

    if (yylex_init(&scanner)) {
        exit(1);
    }

    FILE *fd = fopen(defs_filename, "r");
    if (!fd)
        return NULL;

    yyset_in(fd, scanner);

    if (yyparse(&def, scanner)) {
        exit(1);
    }

    yylex_destroy(scanner);

    return def;
}
