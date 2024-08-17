#ifndef LVAL_H
#define LVAL_H

struct lval;
typedef struct lval lval;
struct lenv;
typedef struct lenv lenv;

#include "lenv.h"
#include "mpc.h"

/* Definition of a Lisp Value */
typedef struct lval {
  int type;

  /* Basic */
  long num;
  char *err;
  char *sym;

  /* Function related */
  lbuiltin builtin;
  lenv *env;
  lval *formals;
  lval *body;

  /* Expression */
  int count;
  struct lval **cell;
} lval;

/* lval types */
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };

/* Methods to create Lisp Values */
lval *lval_num(long x);
lval *lval_err(char *fmt, ...);
lval *lval_sym(char *s);
lval *lval_fun(lbuiltin func);
lval *lval_lambda(lval *formals, lval *body);
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
lval *lval_call(lenv *e, lval *f, lval *a);
lval *lval_eval(lenv *e, lval *v);
lval *lval_eval_sexpr(lenv *e, lval *v);

/* Methods for copying Lisp Values */
lval *lval_copy(lval *v);

/* Methods for printing Lisp Values */
void lval_print(lval *v);
void lval_expr_print(lval *v, char open, char close);
void lval_println(lval *v);

/* Comparison functions */
int lval_eq(lval *x, lval *y);
#endif
