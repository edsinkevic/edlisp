#include "edlisp_semantic.h"
#include "lisp.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEF "def"

typedef struct edlisp_symbol {
  char *name;
  struct edlisp_symbol *next;
} EDLISP_SYMBOL;

EDLISP_SYMBOL *SYMBOLS;


char edlisp_symbol_is(S_EXPR *tree, char *name) {
  return strcmp(tree->string_val, name) == 0;
}

char edlisp_symbol_is_defined(S_EXPR *symbol) {
  for (EDLISP_SYMBOL *cursor = SYMBOLS; cursor != NULL; cursor = cursor->next) {
    if (strcmp(cursor->name, symbol->string_val) == 0) {
      return 1;
    }
  }

  return 0;
}

char edlisp_symbol_is_not_defined(S_EXPR *symbol) {
  return !edlisp_symbol_is_defined(symbol);
}

void edlisp_define_symbol(char *name) {
  assert(name != NULL);
  printf("Defining symbol '%s'...\n", name);

  EDLISP_SYMBOL *sym = malloc(sizeof(EDLISP_SYMBOL *));
  sym->name = strdup(name);
  sym->next = SYMBOLS;
  SYMBOLS = sym;
}

void edlisp_analyze_tree(S_EXPR *parent, S_EXPR *tree);

S_EXPR *edlisp_semantic_analysis(S_EXPR *expr) {
  printf("Running semantic analysis...\n");
  edlisp_analyze_tree(edlisp_make_nil(), expr);
  return NULL;
}

void edlisp_semantic_init() {
  printf("Initializing semantic analysis...\n");

  SYMBOLS = NULL;
  int sn = 5;
  char *symbols[] = {"+", "-", "print", "if", "eq"};

  for (int i = 0; i < sn; ++i) {
    edlisp_define_symbol(symbols[i]);
  }
}

void edlisp_analyze_tree(S_EXPR *parent, S_EXPR *tree) {
  if (tree == edlisp_make_nil()) {
    return;
  }

  if (parent == edlisp_make_nil()) {
  } else {
  }

  if (tree->type == S_CONS) {
    edlisp_analyze_tree(tree, tree->car);
    edlisp_analyze_tree(tree, tree->cdr);
    return;
  }

  if (tree->type == S_MAP) {
    edlisp_analyze_tree(tree, tree->car);
    edlisp_analyze_tree(tree, tree->cdr);
    return;
  }

  if (tree->type == S_NIL) {
    return;
  }
  if (tree->type == S_NUMBER) {
    return;
  }
  if (tree->type == S_SYMBOL) {
    printf("Symbol '%s' detected!\n", tree->string_val);

    if (edlisp_symbol_is(tree, DEF)) {
      S_EXPR *def_body = parent->cdr;
      assert(def_body != NULL);
      assert(def_body->type == S_CONS);

      S_EXPR *head = def_body->car;
      if (head->type != S_SYMBOL) {
        fprintf(stderr, "'def' expects a symbol and a body!\n");
        exit(EXIT_FAILURE);
      }

      edlisp_define_symbol(head->string_val);
    } else if (edlisp_symbol_is_not_defined(tree)) {
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
    edlisp_analyze_tree(tree, tree->key);
    edlisp_analyze_tree(tree, tree->value);
    return;
  }

  printf("Unknown type reached!\n");
}
