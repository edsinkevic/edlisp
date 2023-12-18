#include "lisp.h"
#include "edlisp_io.h"
#include "edlisp_semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern FILE *yyin;

extern int yyparse(S_EXPR **env);

extern int yylineno;
extern char *yytext;

extern void yyerror(S_EXPR **s_expr, char *e) {
  fprintf(stderr, "Error on line %d:\n\t%s: %s\n", yylineno, e, yytext);
}

char parse() {
  S_EXPR *s = edlisp_init_tree();
  if (yyparse(&s)) {
    fprintf(stderr, "Could not parse.\n");
    exit(EXIT_FAILURE);
  }

  if (s == edlisp_make_nil()) {
    printf("Nothing left to parse...\n");
    return 0;
  }
  
  edlisp_semantic_analysis(s);
  return 1;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: edlisp FILE_PATH\n");
    return EXIT_FAILURE;
  }

  yyin = fopen(argv[1], "r");

  if (!yyin) {
    fprintf(stderr, "Could not open file: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  edlisp_semantic_init();

  while (parse()) {}

  return EXIT_SUCCESS;
}
