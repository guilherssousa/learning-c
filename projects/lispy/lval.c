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

// Create a pointer to new *empty* Sexpr lval
lval *lval_sexpr(void) {
  lval *v = malloc(sizeof(lval));

  v->type = LVAL_SEXPR;
  v->count = 0;
  v->cell = NULL;

  return v;
}
