#include <string.h>
#include <stdlib.h>
#include "lisp.h"
#include <assert.h>
#include <malloc.h>

S_EXPR *NIL;

int CURRENT_INDEX = 0;

char edlisp_nil_is(S_EXPR *expr) {
  return expr == NIL;
}

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

S_EXPR *edlisp_attach_location(S_EXPR *expr, int first_line) {
  expr->first_line = first_line;
  return expr;
}
