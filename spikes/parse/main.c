/*
 * main.c
 */

#include <stdio.h>
#include "def.h"
#include "parse.h"
#include "lex.h"

int yyparse(struct def **def, yyscan_t scanner);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s rbac.defs\n", argv[0]);
        exit(1);
    }

    struct def *def;
    yyscan_t scanner;

    if (yylex_init(&scanner)) {
        exit(1);
    }

    yyset_in(fopen(argv[1], "r"), scanner);

    if (yyparse(&def, scanner)) {
        exit(1);
    }

    yylex_destroy(scanner);

    for (struct def *cur = def; cur; cur = cur->next) {
        switch (cur->type) {
        case DEF_USER:
            printf("User definition:\n");
            printf("\tRoles: ");
            for (struct slist *slist = cur->roles; slist; slist = slist->next)
                printf("%s ", slist->s);
            printf("\n");
            printf("\tUsers: ");
            for (struct slist *slist = cur->user.users; slist; slist = slist->next)
                printf("%s ", slist->s);
            printf("\n");
            break;
        case DEF_OBJ:
            printf("Object definition:\n");
            printf("\tRoles: ");
            for (struct slist *slist = cur->roles; slist; slist = slist->next)
                printf("%s ", slist->s);
            printf("\n");
            printf("\tPermissions: ");
            for (struct slist *slist = cur->obj.perms; slist; slist = slist->next)
                printf("%s ", slist->s);
            printf("\n");
            printf("\tObject: %s\n", cur->obj.obj);
            printf("\tRecursive? %s\n", cur->obj.recursive ? "yes" : "no");
            break;
        }
    }
    def_destroy(def);
}
