%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <errno.h>
  #include "lisp.h"


  extern char *yytext;
  extern int yylineno;
  extern int yylex();
  extern FILE *yyin;

  int yyerror(struct S_EXPR **s_expr, char *e);
%}

%parse-param {struct S_EXPR **s_expr}

%union {
  char *l_str;
  char *l_symbol;
  char *l_string;
  char *l_keyword;
  int l_number;
  struct S_EXPR *l_s_expr;
}

%token L_OPENP L_CLOSEP L_DOT L_OPENMAP L_CLOSEMAP L_COMMENTOPEN L_NEWLINE L_EOF

%token <l_number> L_NUMBER
%token <l_symbol> L_SYMBOL
%token <l_keyword> L_KEYWORD
%token <l_string> L_STRING
%type <l_s_expr> s list map pair map_pairs list_items
%start program

%%

program	: s			{ *s_expr = $1; YYACCEPT; }
        | L_EOF     { YYACCEPT; }

s	: L_SYMBOL		{ $$ = edlisp_make_symbol($1); }
	| L_STRING		{ $$ = edlisp_make_string($1); }
	| L_NUMBER		{ $$ = edlisp_make_number($1); }
	| L_KEYWORD     { $$ = edlisp_make_keyword($1); }
	| list          { $$ = $1; }
	| map          { $$ = $1; }

map	 : L_OPENMAP map_pairs L_CLOSEMAP { $$ = $2; }

map_pairs	 : s s { $$ = edlisp_make_map_cons($1, $2, edlisp_make_nil()); }
             | s s map_pairs { $$ = edlisp_make_map_cons($1, $2, $3); }

list	 : L_OPENP list_items L_CLOSEP { $$ = $2; }

list_items: s { $$ = edlisp_make_cons($1, edlisp_make_nil()); }
         | s list_items { $$ = edlisp_make_cons($1, $2); }
         | pair { $$ = $1; }

pair     : s L_DOT s { $$ = edlisp_make_cons($1, $3); }

%%
int yyerror(struct S_EXPR **s_expr, char *e) {
    fprintf(stderr, "Error on line %d:\n\t%s: %s\n", yylineno, e, yytext);
}
