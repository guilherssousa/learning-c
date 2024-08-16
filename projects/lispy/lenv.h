#ifndef LENV_H
#define LENV_H

struct lval;
typedef struct lval lval;

struct lenv;
typedef struct lenv lenv;
typedef lval *(*lbuiltin)(lenv *, lval *);

#include "lval.h"

struct lenv {
  int count;
  char **syms; // List of Symbols
  lval **vals; // List of Vals
};

lenv *lenv_new(void);
void lenv_del(lenv *e);
lval *lenv_get(lenv *e, lval *k);
void lenv_put(lenv *e, lval *k, lval *v);

#endif
