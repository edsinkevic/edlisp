#ifndef EDLISP_SEMANTIC_H
#define EDLISP_SEMANTIC_H

#include "lisp.h"
#include <stdint.h>
#include <bits/types/FILE.h>

typedef S_EXPR *(*EDLISP_EXECUTION)(S_EXPR *args);

typedef struct edlisp_symbol {
  char *name;
  S_EXPR *value;
  struct edlisp_symbol *next;
  EDLISP_EXECUTION execute;
} EDLISP_SYMBOL;

static EDLISP_SYMBOL *SYMBOLS;

S_EXPR *edlisp_eval(S_EXPR *);
void edlisp_semantic_init();
void edlisp_print(S_EXPR *);

#endif
