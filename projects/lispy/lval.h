#ifndef LVAL_H
#define LVAL_H

#include "mpc.h"

/* Definition of a Lisp Value */
typedef struct lval {
  int type;
  long num;

  /* Error and symbol types have some string data */
  char *err;
  char *sym;

  /* Count and pointer to a list of "lval*" */
  int count;
  struct lval **cell;
} lval;

/* lval types */
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR, LVAL_QEXPR };

/* lval error types */
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

/* Methods to create Lisp Values */
lval *lval_num(long x);
lval *lval_err(char *m);
lval *lval_sym(char *s);
lval *lval_sexpr(void);
lval *lval_qexpr(void);

/* Memory handling methods */
lval *lval_add(lval *v, lval *x);
void lval_del(lval *v);

/* Program reading methods */
lval *lval_read(mpc_ast_t *t);
lval *lval_read_num(mpc_ast_t *t);

/* Methods for dealing with cells */
lval *lval_pop(lval *v, int i);
lval *lval_take(lval *v, int i);
lval *lval_join(lval *v, lval *y);

/* Methods for evaluating Lisp Values */
lval *lval_eval(lval *v);
lval *lval_eval_sexpr(lval *v);

/* Methods for printing Lisp Values */
void lval_print(lval *v);
void lval_expr_print(lval *v, char open, char close);
void lval_println(lval *v);
#endif
