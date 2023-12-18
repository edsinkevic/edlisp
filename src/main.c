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

extern void yyerror(struct S_EXPR **s_expr, char *e) {
    fprintf(stderr, "Error on line %d:\n\t%s: %s\n", yylineno, e, yytext);
    fprintf(stderr, "Printing out dot file so far:\n");
    edlisp_print_dot_file(stderr, *s_expr);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: lisp FILE_PATH\n");
        return EXIT_FAILURE;
    }

    yyin = fopen(argv[1], "r");

    if (!yyin) {
        fprintf(stderr, "Could not open file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    struct S_EXPR *s = edlisp_init_tree();
    if (yyparse(&s)) {
        return EXIT_FAILURE;
    }

    edlisp_semantic_analysis(s);

    edlisp_print_dot_file(stdout, s);

    return EXIT_SUCCESS;
}

