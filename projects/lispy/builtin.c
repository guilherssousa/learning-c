#include "lval.h"

#define LASSERT(args, cond, err)                                               \
  if (!(cond)) {                                                               \
    lval_del(args);                                                            \
    return lval_err(err);                                                      \
  }

lval *builtin_op(lval *a, char *op) {
  /* Ensure all arguments are numbers */
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != LVAL_NUM) {
      lval_del(a);
      return lval_err("Cannot operate on non-Number!");
    }
  }

  /* Pop first element */
  lval *x = lval_pop(a, 0);

  /*
    If no arguments and op is sub, perform unary negation
    This handles special values like (- 5) that should
    evaluate to -5.
  */
  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->num = -x->num;
  }

  /* While there are still elements remaining */
  while (a->count > 0) {
    /* Pop next element */
    lval *y = lval_pop(a, 0);

    if (strcmp(op, "+") == 0) {
      x->num += y->num;
    }
    if (strcmp(op, "-") == 0) {
      x->num -= y->num;
    }
    if (strcmp(op, "*") == 0) {
      x->num *= y->num;
    }
    if (strcmp(op, "/") == 0) {
      if (y->num == 0) {
        lval_del(x);
        lval_del(y);
        x = lval_err("Division by Zero!");
        break;
      }
      x->num /= y->num;
    }
    if (strcmp(op, "%") == 0) {
      x->num %= y->num;
    }
    if (strcmp(op, "**") == 0) {
      x->num = pow(x->num, y->num);
    }

    lval_del(y);
  }

  lval_del(a);

  return x;
}

lval *builtin_head(lval *a) {
  /* Check Error Conditions */
  LASSERT(a, a->count == 1, "Function 'head' passed too many arguments.");

  /* Only Q-Expressions should be evaluated by Head */
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
          "Function 'head' passed incorrect types!");

  /* If Q-Expression is empty */
  LASSERT(a, a->cell[0]->count != 0, "Function 'head' passed {}!");

  /* Otherwise take first agrument */
  lval *v = lval_take(a, 0);

  /* Delete all elements that are not 'head' and return */
  while (v->count > 1)
    lval_del(lval_pop(v, 1));

  return v;
}

lval *builtin_tail(lval *a) {
  /* Check Error Conditions */
  LASSERT(a, a->count == 1, "Function 'tail' passed too many arguments.");

  /* Only Q-Expressions should be evaluated by Head */
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
          "Function 'tail' passed incorrect types!");

  /* If Q-Expression is empty */
  LASSERT(a, a->cell[0]->count != 0, "Function 'tail' passed {}!");

  /* Take first agrument */
  lval *v = lval_take(a, 0);

  /* Delete first argument and return */
  lval_del(lval_pop(v, 0));

  return v;
}

/* Transform a S-Expression into a Q-Expression */
lval *builtin_list(lval *a) {
  a->type = LVAL_QEXPR;

  return a;
}

/* Transform a Q-Expression into a S-Expression and evaluate */
lval *builtin_eval(lval *a) {
  LASSERT(a, a->count == 1, "Function 'eval' passed too many arguments!");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
          "Functoin 'eval' passed incorrect type!");

  lval *x = lval_take(a, 0);
  x->type = LVAL_SEXPR;

  return lval_eval(x);
}

/* Join multiple Q-Expressions into one */
lval *builtin_join(lval *a) {
  for (int i = 0; i < a->count; i++) {
    LASSERT(a, a->cell[i]->type == LVAL_QEXPR,
            "Function 'join' passed incorrect type!");
  }

  lval *x = lval_pop(a, 0);

  while (a->count) {
    x = lval_join(x, lval_pop(a, 0));
  }

  lval_del(a);
  return x;
}

lval *builtin(lval *a, char *func) {
  if (strcmp("list", func) == 0)
    return builtin_list(a);

  if (strcmp("head", func) == 0)
    return builtin_head(a);

  if (strcmp("tail", func) == 0)
    return builtin_tail(a);

  if (strcmp("join", func) == 0)
    return builtin_join(a);

  if (strcmp("eval", func) == 0)
    return builtin_eval(a);

  if (strstr("**+-/*%", func))
    return builtin_op(a, func);

  lval_del(a);
  return lval_err("Unknown function");
}
