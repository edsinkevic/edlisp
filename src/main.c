#include "lisp.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern FILE *yyin;

extern int yyparse(S_EXPR **env);


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
    yyparse(&s);

    emit_dot_header(stdout);
    edlisp_walk_tree_dot(edlisp_make_nil(), s, stdout);
    fprintf(stdout, "}}\n");
    return EXIT_SUCCESS;
}
