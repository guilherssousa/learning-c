#include "ltype.h"
#include "lval.h"
#include "parser.h"

#define LASSERT(args, cond, fmt, ...)                                          \
  if (!(cond)) {                                                               \
    lval *err = lval_err(fmt, ##__VA_ARGS__);                                  \
    lval_del(args);                                                            \
    return err;                                                                \
  }

#define LASSERT_TYPE(func, args, index, expert)                                \
  LASSERT(args, args->cell[index]->type == expert,                             \
          "Function '%s' passed incorrect type for argument %i. "              \
          "Got %s, expected %s.",                                              \
          func, index, ltype_name(args->cell[index]->type),                    \
          ltype_name(expert))

#define LASSERT_NUM(func, args, num)                                           \
  LASSERT(args, args->count == num,                                            \
          "Function '%s' passed incorrect number of arguments. "               \
          "Got %i, expected %i.",                                              \
          func, args->count, num)

#define LASSERT_NOT_EMPTY(func, args, index)                                   \
  LASSERT(args, args->cell[index]->count != 0,                                 \
          "Function '%s' passed {} for argument %i.", func, index)

lval *builtin_load(lenv *e, lval *a) {
  LASSERT_NUM("load", a, 1);
  LASSERT_TYPE("load", a, 0, LVAL_STR);

  /* Parse File given by string name */
  mpc_result_t r;
  if (parser_parse_contents(a->cell[0]->str, &r)) {
    /* Read contents */
    lval *expr = lval_read(r.output);
    mpc_ast_delete(r.output);

    /* Evaluate each Expression */
    while (expr->count) {
      lval *x = lval_eval(e, lval_pop(expr, 0));

      /* If Evaluation leads to error, print it */
      if (x->type == LVAL_ERR)
        lval_println(x);

      lval_del(x);
    }

    /* Delete expressoins and arguments */
    lval_del(expr);
    lval_del(a);

    return lval_sexpr();
  } else {
    /* Get Parse error as String */
    char *error = mpc_err_string(r.error);
    mpc_err_delete(r.error);

    /* Create new Error message using it */
    lval *err = lval_err("Could not load Library %s.", error);
    free(error);
    lval_del(a);

    /* Cleanup and return error */
    return err;
  }
}

/* Function to return comparisons between two numbers. */
lval *builtin_cmp(lenv *e, lval *a, char *op) {
  LASSERT_NUM(op, a, 2);

  int r;
  /* Push two comparing values */
  lval *ca = lval_pop(a, 0);
  lval *cb = lval_pop(a, 0);

  if (strcmp(op, ">") == 0) {
    r = ca->num > cb->num;
  }
  if (strcmp(op, "<") == 0) {
    r = ca->num < cb->num;
  }
  if (strcmp(op, "<=") == 0) {
    r = ca->num <= cb->num;
  }
  if (strcmp(op, ">=") == 0) {
    r = ca->num <= cb->num;
  }
  if (strcmp(op, "==") == 0) {
    r = lval_eq(ca, cb);
  }
  if (strcmp(op, "!=") == 0) {
    r = lval_eq(ca, cb);
  }

  lval_del(ca);
  lval_del(cb);
  lval_del(a);

  return lval_num(r);
}

lval *builtin_op(lenv *e, lval *a, char *op) {
  /* Ensure all arguments are numbers */
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != LVAL_NUM) {
      LASSERT_TYPE(op, a, i, LVAL_NUM);
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
    if (strcmp(op, "**") == 0) {
      x->num = pow(x->num, y->num);
    }
    if (strcmp(op, "%") == 0) {
      x->num %= y->num;
    }

    lval_del(y);
  }

  lval_del(a);

  return x;
}

lval *builtin_head(lenv *e, lval *a) {
  LASSERT_NUM("head", a, 1);
  LASSERT_TYPE("head", a, 0, LVAL_QEXPR);
  LASSERT_NOT_EMPTY("head", a, 0);

  /* Otherwise take first agrument */
  lval *v = lval_take(a, 0);

  /* Delete all elements that are not 'head' and return */
  while (v->count > 1)
    lval_del(lval_pop(v, 1));

  return v;
}

lval *builtin_tail(lenv *e, lval *a) {
  LASSERT_NUM("tail", a, 1);
  LASSERT_TYPE("tail", a, 0, LVAL_QEXPR);
  LASSERT_NOT_EMPTY("tail", a, 0);

  /* Take first argument */
  lval *v = lval_take(a, 0);

  /* Delete first argument and return */
  lval_del(lval_pop(v, 0));

  return v;
}

lval *builtin_len(lenv *e, lval *a) {
  LASSERT_NUM("len", a, 1);
  LASSERT_TYPE("len", a, 0, LVAL_QEXPR);

  /* Take the first agrument */
  lval *v = lval_take(a, 0);

  /* Delete element and return */
  lval_del(v);

  return lval_num(v->count);
}

/* Transform a S-Expression into a Q-Expression */
lval *builtin_list(lenv *e, lval *a) {
  a->type = LVAL_QEXPR;

  return a;
}

/* Transform a Q-Expression into a S-Expression and evaluate */
lval *builtin_eval(lenv *e, lval *a) {
  LASSERT_NUM("eval", a, 1);
  LASSERT_TYPE("eval", a, 0, LVAL_QEXPR);

  lval *x = lval_take(a, 0);
  x->type = LVAL_SEXPR;

  return lval_eval(e, x);
}

/* Return the length of a Q-Expression */
lval *builtin_join(lenv *e, lval *a) {
  for (int i = 0; i < a->count; i++) {
    LASSERT_TYPE("join", a, i, LVAL_QEXPR);
  }

  lval *x = lval_pop(a, 0);

  while (a->count) {
    x = lval_join(x, lval_pop(a, 0));
  }

  lval_del(a);
  return x;
}

lval *builtin_var(lenv *e, lval *a, char *func) {
  LASSERT_TYPE(func, a, 0, LVAL_QEXPR);

  lval *syms = a->cell[0];

  for (int i = 0; i < syms->count; i++) {
    LASSERT(a, (syms->cell[i]->type == LVAL_SYM),
            "Function '%s' cannot define non-Symbol. "
            "Got %s, expected %s.",
            func, ltype_name(syms->cell[i]->type), ltype_name(LVAL_SYM));
  }

  LASSERT(a, (syms->count == a->count - 1),
          "Function '%s' passed too many arguments for Symbols. "
          "Got %i, expected %i.",
          func, syms->count, a->count - 1);

  for (int i = 0; i < syms->count; i++) {
    /* Diferentiate between 'def' and 'put'
     * If 'def': Define in global (outermost) environment.
     * If 'put': Define in local environment.
     */

    if (strcmp(func, "def") == 0) {
      lenv_def(e, syms->cell[i], a->cell[i + 1]);
    }
    if (strcmp(func, "=") == 0) {
      lenv_put(e, syms->cell[i], a->cell[i + 1]);
    }
  }

  lval_del(a);
  return lval_sexpr();
}

lval *builtin_def(lenv *e, lval *a) { return builtin_var(e, a, "def"); }

lval *builtin_put(lenv *e, lval *a) { return builtin_var(e, a, "="); }

/* Receive a value, and execute based on result */
lval *builtin_if(lenv *e, lval *a) {
  LASSERT_NUM("if", a, 3);
  LASSERT_TYPE("if", a, 0, LVAL_NUM);
  LASSERT_TYPE("if", a, 1, LVAL_QEXPR);
  LASSERT_TYPE("if", a, 2, LVAL_QEXPR);

  /* This marks both expressions as evaluable */
  lval *x;
  a->cell[1]->type = LVAL_SEXPR;
  a->cell[2]->type = LVAL_SEXPR;

  if (a->cell[0]->num) {
    x = lval_eval(e, lval_pop(a, 1));
  } else {
    x = lval_eval(e, lval_pop(a, 2));
  }

  /* Delete argument list and return */

  lval_del(a);
  return x;
}

/* User-defined functions */
lval *builtin_lambda(lenv *e, lval *a) {
  /* Check for Formal Arguments and Body, both Q-Expressions */
  LASSERT_NUM("lambda", a, 2);
  LASSERT_TYPE("lambda", a, 0, LVAL_QEXPR);
  LASSERT_TYPE("lambda", a, 1, LVAL_QEXPR);

  /* Check first Q-Expression contains only Symbols */
  for (int i = 0; i < a->cell[0]->count; i++) {
    LASSERT(a, (a->cell[0]->cell[i]->type == LVAL_SYM),
            "Cannot define non-Symbol. Got %s, expected %s.",
            ltype_name(a->cell[0]->cell[i]->type), ltype_name(LVAL_SYM));
  }

  /* Pop the first two arguments and pass them to lval_lambda */
  lval *formals = lval_pop(a, 0);
  lval *body = lval_pop(a, 0);
  lval_del(a);

  return lval_lambda(formals, body);
}

/* Basic Arithmetical operations */
lval *builtin_add(lenv *e, lval *a) { return builtin_op(e, a, "+"); }
lval *builtin_sub(lenv *e, lval *a) { return builtin_op(e, a, "-"); }
lval *builtin_mul(lenv *e, lval *a) { return builtin_op(e, a, "*"); }
lval *builtin_div(lenv *e, lval *a) { return builtin_op(e, a, "/"); }
lval *builtin_mod(lenv *e, lval *a) { return builtin_op(e, a, "%"); }
lval *builtin_pow(lenv *e, lval *a) { return builtin_op(e, a, "**"); }

/* Conditional operators */
lval *builtin_eq(lenv *e, lval *a) { return builtin_cmp(e, a, "=="); }
lval *builtin_ne(lenv *e, lval *a) { return builtin_cmp(e, a, "!="); }
lval *builtin_gt(lenv *e, lval *a) { return builtin_cmp(e, a, ">"); }
lval *builtin_lt(lenv *e, lval *a) { return builtin_cmp(e, a, "<"); }
lval *builtin_gte(lenv *e, lval *a) { return builtin_cmp(e, a, ">="); }
lval *builtin_lte(lenv *e, lval *a) { return builtin_cmp(e, a, "<="); }

/* Print values */
lval *builtin_print(lenv *e, lval *a) {
  /* Print each argument followed by a space */
  for (int i = 0; i < a->count; i++) {
    lval_print(a->cell[i]);
    putchar(' ');
  }

  /* Print newline and delete arguments */
  putchar('\n');
  lval_del(a);

  return lval_sexpr();
};

/* Throw an error */
lval *builtin_error(lenv *e, lval *a) {
  LASSERT_NUM("error", a, 1);
  LASSERT_TYPE("error", a, 0, LVAL_STR);

  lval *err = lval_err(a->cell[0]->str);

  lval_del(a);
  return err;
}

void lenv_add_builtin(lenv *e, char *name, lbuiltin func) {
  lval *k = lval_sym(name);
  lval *v = lval_fun(func);

  lenv_put(e, k, v);
  lval_del(k);
  lval_del(v);
}

void lenv_add_builtins(lenv *e) {
  /* List functions */
  lenv_add_builtin(e, "list", builtin_list);
  lenv_add_builtin(e, "head", builtin_head);
  lenv_add_builtin(e, "tail", builtin_tail);
  lenv_add_builtin(e, "eval", builtin_eval);
  lenv_add_builtin(e, "join", builtin_join);
  lenv_add_builtin(e, "len", builtin_len);

  /* Function-related Functions */
  lenv_add_builtin(e, "lambda", builtin_lambda);
  lenv_add_builtin(e, "def", builtin_def);
  lenv_add_builtin(e, "=", builtin_put);

  /* Flow-control operations */
  lenv_add_builtin(e, "if", builtin_if);

  /* Mathematical Functions */
  lenv_add_builtin(e, "+", builtin_add);
  lenv_add_builtin(e, "-", builtin_sub);
  lenv_add_builtin(e, "*", builtin_mul);
  lenv_add_builtin(e, "/", builtin_div);
  lenv_add_builtin(e, "%", builtin_mod);
  lenv_add_builtin(e, "**", builtin_pow);

  /* Conditional operators */
  lenv_add_builtin(e, "==", builtin_eq);
  lenv_add_builtin(e, "!=", builtin_ne);
  lenv_add_builtin(e, ">", builtin_gt);
  lenv_add_builtin(e, "<", builtin_lt);
  lenv_add_builtin(e, ">=", builtin_gte);
  lenv_add_builtin(e, "<=", builtin_lte);

  /* I/O functions */
  lenv_add_builtin(e, "load", builtin_load);
  lenv_add_builtin(e, "print", builtin_print);
  lenv_add_builtin(e, "error", builtin_error);
}
