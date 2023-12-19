%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <errno.h>
  #include "lisp.h"

  extern int yylex();
  int yyerror(struct S_EXPR **s_expr, char *e);
  extern int yylineno;
  #define WITH_LOCATION(V) edlisp_attach_location(V, yylineno);
%}

%parse-param {struct S_EXPR **s_expr}

%union {
  char *l_str;
  char *l_symbol;
  char *l_string;
  char *l_keyword;
  int l_number;
  int line_number;
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

s	: L_SYMBOL		{ $$ = WITH_LOCATION(edlisp_make_symbol($1)); }
	| L_STRING		{ $$ = WITH_LOCATION(edlisp_make_string($1)); }
	| L_NUMBER		{ $$ = WITH_LOCATION(edlisp_make_number($1)); }
	| L_KEYWORD     { $$ = WITH_LOCATION(edlisp_make_keyword($1)); }
	| list          { $$ = $1; }
	| map          { $$ = $1; }

map	 : L_OPENMAP map_pairs L_CLOSEMAP { $$ = $2; }

map_pairs	 : s s { $$ = WITH_LOCATION(edlisp_make_map_cons($1, $2, edlisp_make_nil())); }
             | s s map_pairs { $$ = WITH_LOCATION(edlisp_make_map_cons($1, $2, $3)); }

list	 : L_OPENP list_items L_CLOSEP { $$ = $2; }

list_items: s { $$ = WITH_LOCATION(edlisp_make_cons($1, edlisp_make_nil())); }
         | s list_items { $$ = WITH_LOCATION(edlisp_make_cons($1, $2)); }
         | pair { $$ = $1; }

pair     : s L_DOT s { $$ = WITH_LOCATION(edlisp_make_cons($1, $3)); }

%%
