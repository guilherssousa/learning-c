#include <stdlib.h>
#include <string.h>

#include "builtin.h"
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

/* Create a pointer to a new Number lval */
lval *lval_read_num(mpc_ast_t *t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? lval_num(x) : lval_err("Invalid number");
}

lval *lval_join(lval *x, lval *y) {
  while (y->count) {
    x = lval_add(x, lval_pop(y, 0));
  }

  lval_del(y);
  return x;
}

lval *lval_pop(lval *v, int i) {
  /* Find item at "i" */
  lval *x = v->cell[i];

  /* Shift memory after the item at "i" over the top */
  memmove(&v->cell[i], &v->cell[i + 1], sizeof(lval *) * (v->count - i - 1));

  /* Decrease the count of items in the list */
  v->count--;

  /* Reallocate the memory used */
  v->cell = realloc(v->cell, sizeof(lval *) * v->count);
  return x;
}

lval *lval_take(lval *v, int i) {
  lval *x = lval_pop(v, i);
  lval_del(v);
  return x;
}

/* Receive a Lisp Value and evaluate it */
lval *lval_eval(lval *v) {
  /* Evaluate S-Expressions */
  if (v->type == LVAL_SEXPR) {
    return lval_eval_sexpr(v);
  }

  /* Other types remain the same */
  return v;
}

/* Receives an S-Expression and evaluates its value */
lval *lval_eval_sexpr(lval *v) {
  /* Evaluate children */
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(v->cell[i]);
  }

  /* Error checking */
  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERR) {
      return lval_take(v, i);
    }
  }

  /* Empty Expression */
  if (v->count == 0)
    return v;

  /* Single expression */
  if (v->count == 1)
    return lval_take(v, 0);

  /* Ensure first element is a symbol */
  lval *f = lval_pop(v, 0);
  if (f->type != LVAL_SYM) {
    lval_del(f);
    lval_del(v);
    return lval_err("S-Expression does not start with Symbol!");
  }

  /* Call builtin with operator */
  lval *result = builtin(v, f->sym);
  lval_del(f);

  return result;
}

/* Print a Lisp Value */
void lval_print(lval *v) {
  switch (v->type) {
  case LVAL_NUM:
    printf("%li", v->num);
    break;
  case LVAL_ERR:
    printf("Error: %s", v->err);
    break;
  case LVAL_SYM:
    printf("%s", v->sym);
    break;
  case LVAL_SEXPR:
    lval_expr_print(v, '(', ')');
    break;
  case LVAL_QEXPR:
    lval_expr_print(v, '{', '}');
    break;
  }
}

/* Print Lisp Value and attach a new line */
void lval_println(lval *v) {
  lval_print(v);
  putchar('\n');
}

/* Specialized function to print Expressions */
void lval_expr_print(lval *v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    // Print value contained within
    lval_print(v->cell[i]);

    // Don't print trailing space if last element
    if (i != (v->count - 1)) {
      putchar(' ');
    }
  }
  putchar(close);
}
