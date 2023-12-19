#include "edlisp_eval.h"
#include "lisp.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char edlisp_symbol_is(S_EXPR *tree, char *name) {
  return strcmp(tree->string_val, name) == 0;
}

EDLISP_SYMBOL *edlisp_symbol_get(S_EXPR *symbol) {
  assert(symbol != NULL);
  if (symbol->type != S_SYMBOL) {
    return NULL;
  }

  for (EDLISP_SYMBOL *cursor = SYMBOLS; cursor != NULL; cursor = cursor->next) {
    if (edlisp_symbol_is(symbol, cursor->name)) {
      return cursor;
    }
  }

  return NULL;
}

char edlisp_symbol_is_not_defined(S_EXPR *symbol) {
  return edlisp_symbol_get(symbol) == NULL;
}

void edlisp_define_symbol(char *name, EDLISP_EXECUTION execution, S_EXPR *value) {
  assert(name != NULL);
  printf("Defining symbol '%s'...\n", name);

  EDLISP_SYMBOL *sym = malloc(sizeof(EDLISP_SYMBOL *));
  sym->name = strdup(name);
  sym->next = SYMBOLS;
  sym->value = value;
  sym->execute = execution;
  SYMBOLS = sym;
}

void edlisp_analyze_tree(S_EXPR *parent, S_EXPR *tree);

S_EXPR *execute_plus_op(S_EXPR *a, S_EXPR *b) {
  if (a->type != S_NUMBER || b->type != S_NUMBER) {
    printf("Not a number detected in plus!...");
    return edlisp_make_number(0);
  }
  int sum = a->int_val + b->int_val;
  return edlisp_make_number(sum);
}


S_EXPR *execute_minus_op(S_EXPR *a, S_EXPR *b) {
  if (a->type != S_NUMBER || b->type != S_NUMBER || edlisp_nil_is(b)) {
    printf("Not a number detected in minus!...");
    return edlisp_make_number(0);
  }

  return edlisp_make_number(a->int_val - b->int_val);
}

S_EXPR *execute_plus(S_EXPR *args) {
  if (edlisp_nil_is(args)) {
    return edlisp_make_number(0);
  }

  S_EXPR *head = args->car;
  return execute_plus_op(edlisp_eval(head), execute_plus(args->cdr));
}

S_EXPR *execute_minus(S_EXPR *args) {
  if (edlisp_nil_is(args)) {
    return edlisp_make_number(0);
  }

  S_EXPR *head = args->car;
  return execute_minus_op(edlisp_eval(head), execute_minus(args->cdr));
}

S_EXPR *execute_print(S_EXPR *args) {
  if (edlisp_nil_is(args)) {
    return edlisp_make_nil();
  }

  S_EXPR *result = edlisp_eval(args->car);
  edlisp_print(result);
  execute_print(args->cdr);
  return edlisp_make_nil();
}

S_EXPR *execute_if(S_EXPR *args) {
  return args;
}
S_EXPR *execute_eq   (S_EXPR *args) {
  return args;
}

S_EXPR *execute_def(S_EXPR *args) {
  assert(args->type == S_CONS);

  S_EXPR *head = args->car;
  assert(head != NULL);
  if (head->type != S_SYMBOL) {
    fprintf(stderr, "'def' expects a symbol and a body!\n");
    return edlisp_make_nil();
  }

  assert(head->string_val != NULL);
  S_EXPR *result = edlisp_eval(args->cdr->car);
  edlisp_define_symbol(head->string_val, NULL, result);

  return result;
}

void edlisp_semantic_init() {
  printf("Initializing semantic analysis...\n");

  SYMBOLS = NULL;
  int sn = 6;
  char *symbols[] = {"+", "-", "print", "if", "eq", "def"};
  EDLISP_EXECUTION executions[] = {
    &execute_plus,
    &execute_minus,
    &execute_print,
    &execute_if,
    &execute_eq,
    &execute_def
  };

  for (int i = 0; i < sn; ++i) {
    edlisp_define_symbol(symbols[i], executions[i], NULL);
  }
}

S_EXPR *edlisp_symbol_execute(S_EXPR *s, S_EXPR *args) {
  EDLISP_SYMBOL *symbol = edlisp_symbol_get(s);

  printf("Executing %s...\n", symbol->name);
  assert(symbol != NULL);
  assert(symbol->execute != NULL);
  return symbol->execute(args);
}

S_EXPR *edlisp_eval(S_EXPR *tree) {
  assert(tree != NULL);
  switch (tree->type) {
    case S_CONS:
      S_EXPR *symbol = tree->car;
      S_EXPR *args = tree->cdr;
      return edlisp_symbol_execute(symbol, args);
    case S_SYMBOL:
      printf("Symbol '%s' detected on line %d...\n", tree->string_val, tree->first_line);
      if (edlisp_symbol_is_not_defined(tree)) {
        printf("Symbol '%s' is not defined!\n", tree->string_val);
        return NULL;
      }
      EDLISP_SYMBOL *sym = edlisp_symbol_get(tree);
      return sym->value;
  }

  return tree;
}

void edlisp_print(S_EXPR *tree) {
  assert(tree != NULL);
  switch (tree->type) {
    case S_NUMBER:
      printf("%ld\n", tree->int_val);
      return;
    case S_CONS:
      printf("Printing cons...\n");
      edlisp_print(tree->car);
      return;
    case S_SYMBOL:
      printf(":%s:\n", tree->string_val);
      return;
    case S_STRING:
      printf("%s\n", tree->string_val);
      return;
    case S_NIL:
      printf("nil\n");
      return;
  }

  printf("Printing unimplemented for %d!\n", tree->type);
  return;
}
