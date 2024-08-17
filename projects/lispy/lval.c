#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "ltype.h"
#include "lval.h"

/* create a new number type lval */
lval *lval_num(long x) {
  lval *v = malloc(sizeof(lval));

  v->type = LVAL_NUM;
  v->num = x;

  return v;
}

/* create a new Error type lval */
lval *lval_err(char *fmt, ...) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_ERR;

  /* Create a `va` list and initialize it */
  va_list va;
  va_start(va, fmt);

  /* Allocate 512 bytes of space */
  v->err = malloc(512);

  /* printf the error string with a maximum of 511 characters */
  vsnprintf(v->err, 511, fmt, va);

  /* Reallocate to number of bytes actually used */
  v->err = realloc(v->err, strlen(v->err) + 1);

  /* Cleanup or `va` list */
  va_end(va);

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

/* Create a new Function lval */
lval *lval_fun(lbuiltin func) {
  lval *v = malloc(sizeof(lval));

  v->type = LVAL_FUN;
  v->builtin = func;

  return v;
}

lval *lval_lambda(lval *formals, lval *body) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_FUN;

  /* Set builtin to NULL */
  v->builtin = NULL;

  /* Create a Lambda-specific Environment */
  v->env = lenv_new();

  /* Set Formals and Body */
  v->formals = formals;
  v->body = body;

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
  // Number or Function does not have pointers, so break
  case LVAL_FUN:
    if (!v->builtin) {
      lenv_del(v->env);
      lval_del(v->formals);
      lval_del(v->body);
    }
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
  if (strstr(t->tag, "sexpr")) {
    x = lval_sexpr();
  }
  if (strstr(t->tag, "qexpr")) {
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

/*
 * Check if it's a builtin function; if it is, run it.
 * If not, bind arguments to formats field and evaluate body,
 * using env as an environment, and calling the environment as a parent.
 */
lval *lval_call(lenv *e, lval *f, lval *a) {
  /* If Builtin, simply call that */
  if (f->builtin) {
    return f->builtin(e, a);
  }

  int given = a->count;
  int total = f->formals->count;

  /* While arguments still remain to be processed */
  while (a->count) {
    /* If we ran out of formal arguments to bind */
    if (f->formals->count == 0) {
      lval_del(a);
      return lval_err("Function passed too many arguments."
                      "Got %i, expected %i.",
                      given, total);
    }

    /* Pop the first symbol from the formals */
    lval *sym = lval_pop(f->formals, 0);

    /* Special case to deal with '&' */
    if (strcmp(sym->sym, "&") == 0) {
      /* Ensure '&' is followed by another symbol */
      if (f->formals->count != 1) {
        lval_del(a);
        return lval_err("Function format invalid. "
                        "Symbol '&' not followed by single symbol.");
      }

      /* Next formal should be bound to remaining arguments */
      lval *nsym = lval_pop(f->formals, 0);
      lenv_put(f->env, nsym, builtin_list(e, a));

      lval_del(sym);
      lval_del(nsym);
      break;
    }

    /* Pop the next argument from the list */
    lval *val = lval_pop(a, 0);

    /* Bind a copy into the function's environment */
    lenv_put(f->env, sym, val);

    /*Delete symbol and value */
    lval_del(sym);
    lval_del(val);
  }

  /* Argument list is now bound so we can be cleaned up */
  lval_del(a);

  /* If '&' remains in formal list bind to empty list */
  if (f->formals->count > 0 && strcmp(f->formals->cell[0]->sym, "&") == 0) {
    /* Check to ensure that & is not passed invalidly */
    if (f->formals->count != 2) {
      return lval_err("Function format invalid."
                      "Symbol '&' not followed by a single symbol.");
    }

    /* Pop and delete '&' symbol */
    lval_del(lval_pop(f->formals, 0));

    /* Pop next symbol and create an empty list */
    lval *sym = lval_pop(f->formals, 0);
    lval *val = lval_qexpr();

    /* Bind to the environment and delete */
    lenv_put(f->env, sym, val);
    lval_del(sym);
    lval_del(val);
  }

  /* If all formals have been bound, evaluate */
  if (f->formals->count == 0) {
    /* Set environment parent to evaluation environment */
    f->env->par = e;

    /* Evaluate and return */

    return builtin_eval(f->env, lval_add(lval_sexpr(), lval_copy(f->body)));
  } else {
    /* Otherwise, return partially evaluated function */
    return lval_copy(f);
  }
}

/* Receive a Lisp Value and evaluate it */
lval *lval_eval(lenv *e, lval *v) {
  /* Evaluate Symbol */
  if (v->type == LVAL_SYM) {
    lval *x = lenv_get(e, v);
    lval_del(v);
    return x;
  }

  /* Evaluate S-Expressions */
  if (v->type == LVAL_SEXPR) {
    return lval_eval_sexpr(e, v);
  }

  /* Other types remain the same */
  return v;
}

/* Receives an S-Expression and evaluates its value */
lval *lval_eval_sexpr(lenv *e, lval *v) {
  /* Evaluate children */
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(e, v->cell[i]);
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

  /* Ensure first element is a funtion after evaluation */
  lval *f = lval_pop(v, 0);
  if (f->type != LVAL_FUN) {
    lval *err = lval_err("S-Expression starts with incorrect type."
                         "Got %s, expected %s.",
                         ltype_name(f->type), ltype_name(LVAL_FUN));
    lval_del(f);
    lval_del(v);

    return err;
  }

  /* Call */
  lval *result = lval_call(e, f, v);
  lval_del(f);

  return result;
}

lval *lval_copy(lval *v) {
  lval *x = malloc(sizeof(lval));
  x->type = v->type;

  switch (v->type) {
  /* Copy Functions and Numbers directly */
  case LVAL_NUM:
    x->num = v->num;
    break;
  case LVAL_FUN:
    if (v->builtin) {
      x->builtin = v->builtin;
    } else {
      x->builtin = NULL;
      x->env = lenv_copy(v->env);
      x->formals = lval_copy(v->formals);
      x->body = lval_copy(v->body);
    }
    break;

  /* Copy Strings using malloc and strcopy */
  case LVAL_ERR:
    x->err = malloc(strlen(v->err) + 1);
    strcpy(x->err, v->err);
    break;
  case LVAL_SYM:
    x->sym = malloc(strlen(v->sym) + 1);
    strcpy(x->sym, v->sym);
    break;

  /* Copy Lists by copying each sub-expression */
  case LVAL_SEXPR:
  case LVAL_QEXPR:
    x->count = v->count;
    x->cell = malloc(sizeof(lval *) * x->count);

    for (int i = 0; i < x->count; i++) {
      x->cell[i] = lval_copy(v->cell[i]);
    }

    break;
  }

  return x;
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
  case LVAL_FUN:
    if (v->builtin) {
      printf("<builtin function>");
    } else {
      printf("(lambda ");
      lval_print(v->formals);
      putchar(' ');
      lval_print(v->body);
      putchar(')');
    }
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

int lval_eq(lval *x, lval *y) {
  /*Different types are always unequal*/
  if (x->type != y->type)
    return 0;

  /* Compare based on types */
  switch (x->type) {
  case LVAL_NUM:
    return x->num == y->num;
  case LVAL_ERR:
    return strcmp(x->err, y->err);
  case LVAL_SYM:
    return strcmp(x->sym, y->sym);

  /* If value is a list, compare every individual element */
  case LVAL_SEXPR:
  case LVAL_QEXPR:
    if (x->count != y->count)
      return 0;
    for (int i = 0; i < x->count; i++) {
      if (!lval_eq(x->cell[i], y->cell[i]))
        return 0;
    }

    return 1;
    break;
  }

  return 0;
}
