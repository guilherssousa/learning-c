#include <stdlib.h>
#include <string.h>

#include "lval.h"

/* create a new number type lval */
lval *lval_num(long x) {
  lval *v = malloc(sizeof(lval));

  v->type = LVAL_NUM;
  v->num = x;

  return v;
}

/* create a new Error type lval */
lval *lval_err(char *m) {
  lval *v = malloc(sizeof(lval));

  v->type = LVAL_ERR;
  v->err = malloc(strlen(m) + 1);
  strcpy(v->err, m);

  return v;
}

/* Create a new Symbol lval */
lval *lval_sym(char *s) {
  lval *v = malloc(sizeof(lval));

  v->type = LVAL_SYM;
  v->sym = malloc(strlen(s) + 1);
  strcpy(v->sym, s);

  return v;
}

/* Create a pointer to new *empty* Sexpr lval */
lval *lval_sexpr(void) {
  lval *v = malloc(sizeof(lval));

  v->type = LVAL_SEXPR;
  v->count = 0;
  v->cell = NULL;

  return v;
}

/* Create a pointer to a new *empty* Qexpr lval */
lval *lval_qexpr(void) {
  lval *v = malloc(sizeof(lval));

  v->type = LVAL_QEXPR;
  v->count = 0;
  v->cell = NULL;

  return v;
}

/* Expand Lisp Value */
lval *lval_add(lval *v, lval *x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval *) * v->count);
  v->cell[v->count - 1] = x;

  return v;
}

/* Remove Lisp Value from memory */
void lval_del(lval *v) {
  switch (v->type) {
  // Number does not have pointers, so break
  case LVAL_NUM:
    break;

  // For Error or Symbol, free the string data
  case LVAL_ERR:
    free(v->err);
    break;
  case LVAL_SYM:
    free(v->sym);
    break;

  // For Symbol Expression, delete all elements inside
  case LVAL_QEXPR:
  case LVAL_SEXPR:
    for (int i = 0; i < v->count; i++) {
      lval_del(v->cell[i]);
    }
    //
    // Free the memory used to store pointers
    free(v->cell);
    break;
  }

  // Free the memory alocated for the lisp value
  free(v);
}

/* Create a pointer to a new Number lval */
lval *lval_read_num(mpc_ast_t *t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? lval_num(x) : lval_err("Invalid number");
}

/* Read the AST and return a Lisp Value */
lval *lval_read(mpc_ast_t *t) {
  /*   If Symbol or Number, return conversion to that type */
  if (strstr(t->tag, "number")) {
    return lval_read_num(t);
  }
  if (strstr(t->tag, "symbol")) {
    return lval_sym(t->contents);
  }

  /*   If root or Sexpr, create empty list */
  lval *x = NULL;
  if (strcmp(t->tag, ">") == 0) {
    x = lval_sexpr();
  }
  if (strcmp(t->tag, "sexpr")) {
    x = lval_sexpr();
  }
  if (strcmp(t->tag, "qexpr")) {
    x = lval_qexpr();
  }

  // Fill list with any valid expressions
  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0)
      continue;
    if (strcmp(t->children[i]->contents, ")") == 0)
      continue;
    if (strcmp(t->children[i]->contents, "{") == 0)
      continue;
    if (strcmp(t->children[i]->contents, "}") == 0)
      continue;
    if (strcmp(t->children[i]->tag, "regex") == 0)
      continue;

    x = lval_add(x, lval_read(t->children[i]));
  }

  return x;
}
