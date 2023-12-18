#ifndef EDLISP_SEMANTIC_H
#define EDLISP_SEMANTIC_H

#include "lisp.h"
#include <stdint.h>
#include <bits/types/FILE.h>

S_EXPR *edlisp_semantic_analysis(S_EXPR *);
void edlisp_semantic_init();

#endif
