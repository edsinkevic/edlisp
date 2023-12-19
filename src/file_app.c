#include "lisp.h"
#include "edlisp_eval.h"
#include "file_app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern FILE *yyin;

extern int yyparse(S_EXPR **env);

extern int yylineno;
extern char *yytext;

char parse() {
  S_EXPR *s = edlisp_init_tree();
  if (yyparse(&s)) {
    fprintf(stderr, "Could not parse.\n");
    return 0;
  }

  if (edlisp_nil_is(s)) {
    return 0;
  }
  
  S_EXPR *result = edlisp_eval(s);
  return 1;
}

void run_file(char *file_name) {
  yyin = fopen(file_name, "r");

  if (!yyin) {
    fprintf(stderr, "Could not open file: %s\n", strerror(errno));
    exit(1);
  }

  edlisp_semantic_init();

  while (parse()) {}
}

