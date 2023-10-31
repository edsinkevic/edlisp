#ifndef LISP_H
#define LISP_H

#include <stdint.h>
#include <bits/types/FILE.h>

typedef enum {
    S_NUMBER,
    S_KEYWORD,
    S_SYMBOL,
    S_MAP,
    S_MAP_PAIR,
    S_STRING,
    S_CONS,
    S_NIL
} S_EXPR_TYPE;

typedef struct S_EXPR {
    S_EXPR_TYPE type;
    int id;
    union {
        int64_t int_val; /* NUMBER */
        char bool_val; /* BOOL */
        char char_val; /* CHAR */
        char *string_val; /* STRING */
        char *keyword_val; /* KEYWORD */
        char *symbol_val; /* SYMBOL */
        struct { /* CONS */
            struct S_EXPR *car;
            struct S_EXPR *cdr;
        };
        struct {
            struct S_EXPR *key;
            struct S_EXPR *value;
        };
    };
} S_EXPR;

S_EXPR *edlisp_make_symbol(const char *name);

S_EXPR *edlisp_make_number(int64_t val);

S_EXPR *edlisp_make_keyword(char *val);

S_EXPR *edlisp_make_string(char *val);

S_EXPR *edlisp_make_cons(S_EXPR *car, S_EXPR *cdr);

S_EXPR *edlisp_make_nil();

S_EXPR *edlisp_init_tree();

S_EXPR *edlisp_make_map_pair(S_EXPR *key, S_EXPR *value);

S_EXPR *edlisp_make_map_cons(S_EXPR *key, S_EXPR *value, S_EXPR *cdr);

#endif
