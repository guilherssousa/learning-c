#include "lenv.h"
#include "lval.h"
#include <stdio.h>

lenv *lenv_new(void) {
  lenv *e = malloc(sizeof(lenv));

  e->par = NULL;
  e->count = 0;
  e->syms = NULL;
  e->vals = NULL;

  return e;
}

void lenv_del(lenv *e) {
  for (int i = 0; i < e->count; i++) {
    free(e->syms[i]);
    lval_del(e->vals[i]);
  }

  free(e->syms);
  free(e->vals);
  free(e);
}

lval *lenv_get(lenv *e, lval *k) {
  /* Iterate over all items in environment */
  for (int i = 0; i < e->count; i++) {
    /* Check if the stored string matches the symbol string
    If it does, return a copy of the value */
    if (strcmp(e->syms[i], k->sym) == 0) {
      return lval_copy(e->vals[i]);
    }
  }

  /* If no Symbol, check in parent; otherwise, error */
  if (e->par) {
    return lenv_get(e->par, k);
  } else {
    return lval_err("unbound symbol '%s'", k->sym);
  }
}

lenv *lenv_copy(lenv *e) {
  lenv *n = malloc(sizeof(lenv));

  n->par = e->par;
  n->count = e->count;
  n->syms = malloc(sizeof(char *) * n->count);
  n->vals = malloc(sizeof(lval *) * n->count);

  for (int i = 0; i < e->count; i++) {
    n->syms[i] = malloc(strlen(e->syms[i]) + 1);
    strcpy(n->syms[i], e->syms[i]);
    n->vals[i] = lval_copy(e->vals[i]);
  }

  return n;
}

/* Define variable in local environment */
void lenv_put(lenv *e, lval *k, lval *v) {
  /* Iterate over all items in environment
    This is to see if variable already exists */
  for (int i = 0; i < e->count; i++) {
    /* If variable is found delete item at that position
      And replace with variable supplied by user */
    if (strcmp(e->syms[i], k->sym) == 0) {
      lval_del(e->vals[i]);
      e->vals[i] = lval_copy(v);
      return;
    }
  }

  /* If no existing entry found, allocate space for new entry */
  e->count++;
  e->vals = realloc(e->vals, sizeof(lval *) * e->count);
  e->syms = realloc(e->syms, sizeof(char *) * e->count);

  /* Copy contents of lval and symbol string into new location */
  e->vals[e->count - 1] = lval_copy(v);
  e->syms[e->count - 1] = malloc(strlen(k->sym) + 1);
  strcpy(e->syms[e->count - 1], k->sym);
}

/* Define variable in global (outermost) environment */
void lenv_def(lenv *e, lval *k, lval *v) {
  /* Iterate till e has no parent */
  while (e->par) {
    e = e->par;
  }

  /* Put value in e */
  lenv_put(e, k, v);
}
