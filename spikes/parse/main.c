/*
 * main.c
 */

#include <stdio.h>
#include "def.h"
#include "parse.h"
#include "lex.h"

int yyparse(struct def **def, yyscan_t scanner);

int main(void) {
    struct def *def;
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (yylex_init(&scanner)) {
        exit(1);
    }

    state = yy_scan_string("user: role1,role2 user1", scanner);
    //state = yy_scan_string("object: role1 r,w /usr", scanner);

    if (yyparse(&def, scanner)) {
        exit(1);
    }

    yy_delete_buffer(state, scanner);
    yylex_destroy(scanner);

    switch (def->type) {
    case DEF_USER:
        printf("User definition:\n");
        printf("\tRoles: ");
        for (struct slist *slist = def->roles; slist; slist = slist->next)
            printf("%s ", slist->s);
        printf("\n");
        printf("\tUsers: ");
        for (struct slist *slist = def->user.users; slist; slist = slist->next)
            printf("%s ", slist->s);
        printf("\n");
        break;
    case DEF_OBJ:
        printf("Object definition:\n");
        printf("\tRoles: ");
        for (struct slist *slist = def->roles; slist; slist = slist->next)
            printf("%s ", slist->s);
        printf("\n");
        printf("\tPermissions: ");
        for (struct slist *slist = def->obj.perms; slist; slist = slist->next)
            printf("%s ", slist->s);
        printf("\n");
        printf("\tObject: %s\n", def->obj.obj);
        break;
    }

}
