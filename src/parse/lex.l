%{
/*
 * lex.l - Tokenizer.
 */

#include <stdio.h>
#include <string.h>
#include "def.h"
#include "parse.h"
%}
/*
 * Definitions
 */
%option outfile="lex.c" header-file="lex.h"
%option warn nodefault
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

WHITESPACE      [ \r\n\t]*

USER "user:"
OBJ  "object:"

COMMA ","

RECURSIVE       "-r"
PATH            \/[a-zA-Z0-9_.\\\-/]*
PERM            [rwx]
ID              [a-zA-Z0-9_.\\\-/]+

%%

{WHITESPACE}    { /* skip */ }
{USER}          { return TOKEN_USER; }
{OBJ}           { return TOKEN_OBJ; }
{COMMA}         { return TOKEN_COMMA; }
{RECURSIVE}     { return TOKEN_RECURSIVE; }
{PATH}          { yylval->s = strdup(yytext); return TOKEN_PATH; }
{PERM}          { yylval->s = strdup(yytext); return TOKEN_PERM; }
{ID}            { yylval->s = strdup(yytext); return TOKEN_ID; }
.               { }
