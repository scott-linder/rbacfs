/*
 *  lib.c
 *      DESCRIPTION: Controls the parsing of rbacs.def
 */

#include <stdio.h>
#include "def.h"
#include "parse.h"
#include "lex.h"

int yyparse(struct def **def, yyscan_t scanner);

/*
 *  rbac_parse_defs
 *      IN: filename of rbac definitions
 *      OUT: definitions list
 *      DESCRIPTION: Receives the rbac definitions filename (created by user) and sends it to lex/bison parser to create the definitions list.
 */
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
