//
// Created by edvin on 23.10.31.
//

#include <stdio.h>
#include "lisp.h"


void emit_dot_header(FILE *f) {
    fprintf(f, "graph \"\"\n");
    fprintf(f, "{\n");
    fprintf(f, "node [fontname=\"Helvetica,Arial,sans-serif\"]\n");
    fprintf(f, "edge [fontname=\"Helvetica,Arial,sans-serif\"]\n");
    fprintf(f, "label=\"AST\"\n");

    fprintf(f, "subgraph rootnode\n{\nlabel=\"ROOT\"\n");
}

void edlisp_walk_tree_dot(S_EXPR *parent, S_EXPR *tree, FILE *f) {
    if (tree == edlisp_make_nil()) {
        return;
    }

    if (parent == edlisp_make_nil()) {
        fprintf(f, "node%d ;\n", tree->id);
    } else {
        fprintf(f, "node%d -- node%d ;\n", parent->id, tree->id);
    }

    if (tree->type == S_CONS) {
        edlisp_walk_tree_dot(tree, tree->car, f);
        edlisp_walk_tree_dot(tree, tree->cdr, f);
        fprintf(f, "node%d [label=\"Cons\"] ;\n", tree->id);
        return;
    }

    if (tree->type == S_MAP) {
        edlisp_walk_tree_dot(tree, tree->car, f);
        edlisp_walk_tree_dot(tree, tree->cdr, f);
        fprintf(f, "node%d [label=\"Map\"] ;\n", tree->id);
        return;
    }

    if (tree->type == S_NIL) {
        return;
    }
    if (tree->type == S_NUMBER) {
        fprintf(f, "node%d [label=\"Number %ld\"] ;\n", tree->id, tree->int_val);
        return;
    }
    if (tree->type == S_SYMBOL) {
        fprintf(f, "node%d [label=\"Symbol %s\"] ;\n", tree->id, tree->string_val);
        return;
    }
    if (tree->type == S_STRING) {
        fprintf(f, "node%d [label=\"String %s\"] ;\n", tree->id, tree->string_val);
        return;
    }
    if (tree->type == S_KEYWORD) {
        fprintf(f, "node%d [label=\"Keyword %s\"] ;\n", tree->id, tree->keyword_val);
        return;
    }
    if (tree->type == S_MAP_PAIR) {
        edlisp_walk_tree_dot(tree, tree->key, f);
        edlisp_walk_tree_dot(tree, tree->value, f);
        fprintf(f, "node%d [label=\"Map pair\"] ;\n", tree->id);
        return;
    }

    printf("Unknown type reached!\n");
}

void edlisp_print_dot_file(FILE *f, S_EXPR *s_expr) {
    emit_dot_header(f);
    edlisp_walk_tree_dot(edlisp_make_nil(), s_expr, f);
    fprintf(f, "}}\n");
}
