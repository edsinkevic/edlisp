#include "cli_app.h"
#include "lisp.h"
#include "edlisp_eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 6400

char INPUT_BUFFER[INPUT_BUFFER_SIZE];

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern FILE *yyin;

extern int yyparse(S_EXPR **env);

extern int yylineno;
extern char *yytext;

extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

extern void yyerror(S_EXPR **s_expr, char *e) {
  fprintf(stderr, "Error on line %d:\n\t%s: %s\n", yylineno, e, yytext);
}

char cli_parse() {
  S_EXPR *s = edlisp_init_tree();
  if (yyparse(&s)) {
    fprintf(stderr, "Could not parse.\n");
    return 0;
  }

  if (edlisp_nil_is(s)) {
    return 0;
  }
  
  S_EXPR *result = edlisp_eval(s);
  edlisp_print(result);
  return 1;
}

void run_cli() {
  edlisp_semantic_init();

  while (1) {
    printf("\n$/ ");

    if (!fgets(INPUT_BUFFER, INPUT_BUFFER_SIZE, stdin)) {
      if (errno) {
        fprintf(stderr, "Could not read input: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
      }
      break;
    }

    YY_BUFFER_STATE buffer = yy_scan_string(INPUT_BUFFER);

    while(cli_parse()) {}
    yy_delete_buffer(buffer);
  }
}
