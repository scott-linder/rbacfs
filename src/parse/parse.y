%{
/*
 * parse.y - Parser.
 */

#include "def.h"
#include "parse.h"
#include "lex.h"
#include "../list/lib.h"

int yyerror(struct def **def, yyscan_t scanner, const char *msg) {
    fprintf(stderr, "error: %s\n", msg);
    return 0;
}
%}

%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

%output "parse.c"
%defines "parse.h"

%define api.pure
%lex-param { yyscan_t scanner }
%parse-param { struct def **def }
%parse-param { yyscan_t scanner }

%union {
    char *s;
    struct list *list;
    struct def *def;
}

/*
 * Bison/token definitions
 *
 */

%token TOKEN_USER;
%token TOKEN_OBJ;
%token TOKEN_COMMA;
%token TOKEN_RECURSIVE;
%token <s> TOKEN_PERM;
%token <s> TOKEN_ID;
%token <s> TOKEN_PATH;

%type <list> perm_list
%type <list> id_list
%type <def> def_list
%type <def> def

%%

/*
 * Grammer Rules
 */

input
    : def_list { *def = $1; }
    ;

perm_list
    : TOKEN_PERM { $$ = LIST_INIT; list_append(&$$, $1); }
    | perm_list TOKEN_COMMA TOKEN_PERM { list_append(&$1, $3); }
    ;

id_list
    : TOKEN_ID { $$ = LIST_INIT; list_append(&$$, $1); }
    | id_list TOKEN_COMMA TOKEN_ID { list_append(&$1, $3); }
    ;

def_list
    : def
    | def_list def { def_append($1, $2); }
    ;

def
    : TOKEN_USER id_list id_list {
        $$ = def_create(DEF_USER);
        $$->roles = $2;
        $$->user.users = $3;
    }
    | TOKEN_OBJ id_list perm_list TOKEN_RECURSIVE TOKEN_PATH {
        $$ = def_create(DEF_OBJ);
        $$->roles = $2;
        $$->obj.perms = $3;
        $$->obj.obj = $5;
        $$->obj.recursive = 1;
    }
    | TOKEN_OBJ id_list perm_list TOKEN_PATH {
        $$ = def_create(DEF_OBJ);
        $$->roles = $2;
        $$->obj.perms = $3;
        $$->obj.obj = $4;
        $$->obj.recursive = 0;
    }
    ;
