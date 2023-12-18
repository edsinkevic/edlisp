#include "edlisp_semantic.h"
#include <assert.h>
#include <string.h>

typedef struct edlisp_symbol {
  char *name;
  struct edlisp_symbol *next;
} EDLISP_SYMBOL;

EDLISP_SYMBOL *SYMBOLS;

char edlisp_symbol_is_defined(S_EXPR *symbol) {
  for (EDLISP_SYMBOL *cursor = SYMBOL; cursor != NULL; cursor->next) {
    if (strcmp(cursor->name, symbol->string_val) == 0) {
      return 1;
    }
  }

  return 0;
}

char edlisp_symbol_is_not_defined(S_EXPR *symbol) {
  return !edlisp_symbol_is_defined(symbol);
}

void edlisp_symbol_must_be_undefined(char *name) {
  for (EDLISP_SYMBOL *cursor = SYMBOL; cursor != NULL; cursor->next) {
    if (strcmp(cursor->name, name) == 0) {
      printf("Symbol '%s' defined twice!\n");
      exit(1);
    }
  }
}

void edlisp_define_symbol(char *name) {
  assert(name != NULL);
  edlisp_symbol_must_be_undefined(name);

  EDLISP_SYMBOL *sym = malloc(sizeof(EDLISP_SYMBOL));
  sym->name = name;
  sym->next = SYMBOLS;
  SYMBOLS = sym;
}

void edlisp_analyze_tree(S_EXPR *parent, S_EXPR *tree, FILE *f);

S_EXPR *edlisp_semantic_analysis(S_EXPR *expr) {
  printf("Running semantic analysis...\n");

  SYMBOLS = NULL;
  symbols = {"+", "-", "print", "if"};
  sn = 4;

  for (int i = 0; i < sn; ++i) {
    printf("Defining symbol '%s'...\n", symbols[i]);
    edlisp_define_symbol(symbols[i]);
  }

  edlisp_analyze_tree(NULL, expr);
}

void edlisp_analyze_tree(S_EXPR *parent, S_EXPR *tree) {
  if (tree == edlisp_make_nil()) {
    return;
  }

  if (parent == edlisp_make_nil()) {
  } else {
  }

  if (tree->type == S_CONS) {
    edlisp_walk_tree_dot(tree, tree->car, f);
    edlisp_walk_tree_dot(tree, tree->cdr, f);
    return;
  }

  if (tree->type == S_MAP) {
    edlisp_walk_tree_dot(tree, tree->car, f);
    edlisp_walk_tree_dot(tree, tree->cdr, f);
    return;
  }

  if (tree->type == S_NIL) {
    return;
  }
  if (tree->type == S_NUMBER) {
    return;
  }
  if (tree->type == S_SYMBOL) {
    if (edlisp_symbol_is_not_defined(tree)) {
      printf("Symbol '%s' is not defined!\n", tree->string_val);
      exit(1);
    }

    return;
  }
  if (tree->type == S_STRING) {
    return;
  }
  if (tree->type == S_KEYWORD) {
    return;
  }
  if (tree->type == S_MAP_PAIR) {
    edlisp_walk_tree_dot(tree, tree->key, f);
    edlisp_walk_tree_dot(tree, tree->value, f);
    return;
  }

  printf("Unknown type reached!\n");
}
