#include <string.h>
#include <stdlib.h>
#include "lisp.h"
#include <assert.h>
#include <malloc.h>

S_EXPR *NIL;

int CURRENT_INDEX = 0;

static S_EXPR *edlisp_create_expr(S_EXPR_TYPE type) {
    S_EXPR *s = calloc(1, sizeof(*s));
    assert(s != NULL);

    s->type = type;
    s->id = CURRENT_INDEX++;
    return s;
}


S_EXPR *edlisp_make_symbol(const char *name) {
    assert(name != NULL);

    S_EXPR *s = edlisp_create_expr(S_SYMBOL);
    s->string_val = strdup(name);
    return s;
}

S_EXPR *edlisp_make_number(int64_t val) {
    S_EXPR *s = edlisp_create_expr(S_NUMBER);
    s->int_val = val;
    return s;
}

S_EXPR *edlisp_make_keyword(char *val) {
    S_EXPR *s = edlisp_create_expr(S_KEYWORD);
    s->keyword_val = strdup(val);
    return s;
}

S_EXPR *edlisp_make_string(char *val) {
    S_EXPR *s = edlisp_create_expr(S_STRING);

    if (strcmp(val, "\"\"") == 0) {
        s->string_val = strdup("");
        return s;
    }

    char *token = strtok(val, "\"");
    s->string_val = strdup(token);
    return s;
}

S_EXPR *edlisp_make_cons(S_EXPR *car, S_EXPR *cdr) {
    S_EXPR *s = edlisp_create_expr(S_CONS);
    s->car = car;
    s->cdr = cdr;
    return s;
}

S_EXPR *edlisp_make_nil() {
    return NIL;
}

S_EXPR *edlisp_init_tree() {
    NIL = edlisp_create_expr(S_NIL);
    return NIL;
}

S_EXPR *edlisp_make_map_pair(S_EXPR *key, S_EXPR *value) {
    S_EXPR *s = edlisp_create_expr(S_MAP_PAIR);
    s->key = key;
    s->value = value;
    return s;
}

S_EXPR *edlisp_make_map_cons(S_EXPR *key, S_EXPR *value, S_EXPR *cdr) {
    S_EXPR *s = edlisp_create_expr(S_MAP);
    S_EXPR *s_pair = edlisp_create_expr(S_MAP_PAIR);
    s_pair->key = key;
    s_pair->value = value;
    s->car = s_pair;
    s->cdr = cdr;
    return s;
}

void emit_dot_header(FILE *f) {
    fprintf(f, "graph \"\"\n");
    fprintf(f, "{\n");
    fprintf(f, "node [fontname=\"Helvetica,Arial,sans-serif\"]\n");
    fprintf(f, "edge [fontname=\"Helvetica,Arial,sans-serif\"]\n");
    fprintf(f, "label=\"AST\"\n");

    fprintf(f, "subgraph rootnode\n{\nlabel=\"ROOT\"\n");
}

void edlisp_walk_tree_dot(S_EXPR *parent, S_EXPR *tree, FILE *f) {
    if (tree == NIL) {
        return;
    }

    if (parent == NIL) {
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