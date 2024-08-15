#ifndef LVAL_H
#define LVAL_H

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
void lval_del(lval *v);
lval *lval_add(lval *v, lval *x);

#endif
