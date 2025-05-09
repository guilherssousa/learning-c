#ifndef BUILTIN_H
#define BUILTIN_H

#include "lval.h"

/* Run program files */
lval *builtin_load(lenv *e, lval *a);

/* Arithmetics */
lval *builtin_op(lenv *e, lval *a, char *op);

/* Comparisons */
lval *builtin_cmp(lenv *e, lval *a, char *op);

/* Language functions */
lval *builtin_head(lenv *e, lval *a);
lval *builtin_tail(lenv *e, lval *a);
lval *builtin_len(lenv *e, lval *a);
lval *builtin_list(lenv *e, lval *a);
lval *builtin_eval(lenv *e, lval *a);
lval *builtin_join(lenv *e, lval *a);

/* Variables, functions, etc. */
lval *builtin_var(lenv *e, lval *a, char *func);
lval *builtin_def(lenv *e, lval *a);
lval *builtin_put(lenv *e, lval *a);
lval *builtin_lambda(lenv *e, lval *a);

/* Basic Arithmetical operations */
lval *builtin_add(lenv *e, lval *a);
lval *builtin_sub(lenv *e, lval *a);
lval *builtin_mul(lenv *e, lval *a);
lval *builtin_div(lenv *e, lval *a);
lval *builtin_mod(lenv *e, lval *a);
lval *builtin_pow(lenv *e, lval *a);

/* Basic comparision operations */
lval *builtin_eq(lenv *e, lval *a);
lval *builtin_ne(lenv *e, lval *a);
lval *builtin_gt(lenv *e, lval *a);
lval *builtin_lt(lenv *e, lval *a);
lval *builtin_gte(lenv *e, lval *a);
lval *builtin_lte(lenv *e, lval *a);

/* Conditional control operations */
lval *builtin_if(lenv *e, lval *a);

/* I/O Functions */
lval *builtin_print(lenv *e, lval *a);

/* Assemble all those stuff together */
void lenv_add_builtin(lenv *e, char *name, lbuiltin func);
void lenv_add_builtins(lenv *e);
#endif
