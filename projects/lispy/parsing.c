#include <stdio.h>
#include <stdlib.h>

#include <editline/history.h>
#include <editline/readline.h>

#include "mpc.h"

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

// lval types
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR };

// lval error types
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

// create a new number type lval
lval *lval_num(long x) {
  lval *v = malloc(sizeof(lval));

  v->type = LVAL_NUM;
  v->num = x;

  return v;
}

lval *lval_add(lval *v, lval *x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval *) * v->count);
  v->cell[v->count - 1] = x;

  return v;
}

// create a new error type lval
lval *lval_err(char *m) {
  lval *v = malloc(sizeof(lval));

  v->type = LVAL_ERR;
  v->err = malloc(strlen(m) + 1);
  strcpy(v->err, m);

  return v;
}

// Create a pointer to a new Symbol lval
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

lval *lval_read_num(mpc_ast_t *t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? lval_num(x) : lval_err("Invalid number");
}

lval *lval_read(mpc_ast_t *t) {
  // If Symbol or Nummber, return conversion to that type
  if (strstr(t->tag, "number")) {
    return lval_read_num(t);
  }
  if (strstr(t->tag, "symbol")) {
    return lval_sym(t->contents);
  }

  // If root or sexpr then create empty list
  lval *x = NULL;
  if (strcmp(t->tag, ">") == 0) {
    x = lval_sexpr();
  }
  if (strcmp(t->tag, "sexpr")) {
    x = lval_sexpr();
  }

  // Fill list with any valid expressions
  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->contents, ")") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->tag, "regex") == 0) {
      continue;
    }
    x = lval_add(x, lval_read(t->children[i]));
  }

  return x;
}

void lval_del(lval *v) {
  switch (v->type) {
  // Number does not have pointers, so break
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

void lval_expr_print(lval *v, char open, char close);

// Debug function
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
  case LVAL_SEXPR:
    lval_expr_print(v, '(', ')');
    break;
  }
}

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

// Utility
void lval_println(lval *v) {
  lval_print(v);
  putchar('\n');
}

// Check operator and run operation
// lval *eval_op(lval *x, char *op, lval *y) {
//   if (x->type == LVAL_ERR) {
//     return x;
//   }
//   if (y->type == LVAL_ERR) {
//     return y;
//   }
//
//   if (strcmp(op, "+") == 0) {
//     return lval_num(x->num + y->num);
//   }
//   if (strcmp(op, "-") == 0) {
//     return lval_num(x->num - y->num);
//   }
//   if (strcmp(op, "*") == 0) {
//     return lval_num(x->num * y->num);
//   }
//   if (strcmp(op, "/") == 0) {
//     return y->num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x->num / y->num);
//   }
//   if (strcmp(op, "%") == 0) {
//     return lval_num(x->num % y->num);
//   }
//   if (strcmp(op, "^") == 0) {
//     return lval_num(pow(x->num, y->num));
//   }
//
//   return lval_err(LERR_BAD_OP);
// }
//
// lval *eval(mpc_ast_t *t) {
//   // If node tag is a number, cast and return
//   if (strstr(t->tag, "number")) {
//     errno = 0;
//     long x = strtol(t->contents, NULL, 10);
//     return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
//   }
//
//   // Ignore ( when expr is not number
//   char *op = t->children[1]->contents;
//   lval *x = eval(t->children[2]);
//
//   // Iterate remaining children and combine
//   int i = 3;
//
//   while (strstr(t->children[i]->tag, "expr")) {
//     x = eval_op(x, op, eval(t->children[i]));
//     i++;
//   }
//
//   return x;
// }

int number_of_nodes(mpc_ast_t *t) {
  if (t->children_num == 0) {
    return 1;
  }

  if (t->children_num >= 1) {
    int total = 1;
    for (int i = 0; i < t->children_num; i++) {
      total = total + number_of_nodes(t->children[i]);
    }
  }

  return 0;
}

int main(void) {
  // Parsers definition
  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Symbol = mpc_new("symbol");
  mpc_parser_t *Sexpr = mpc_new("sexpr");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Lispy = mpc_new("lispy");

  /* define the language syntax */

  mpca_lang(MPCA_LANG_DEFAULT, "                                          \
    number : /-?[0-9]+/ ;                    \
    symbol : '+' | '-' | '*' | '/' ;         \
    sexpr  : '(' <expr>* ')' ;               \
    expr   : <number> | <symbol> | <sexpr> ; \
    lispy  : /^/ <expr>* /$/ ;               \
  ",
            Number, Symbol, Sexpr, Expr, Lispy);

  printf("Lispy V0.1\n");
  printf("Press Ctrl+C to Exit\n");

  while (1) {
    /* output message */
    char *input = readline("lispy > ");

    /* Add input to hsitory */
    add_history(input);

    mpc_result_t r;

    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      // lval result = eval(r.output);
      //
      // lval_println(result);
      //

      lval *x = lval_read(r.output);
      lval_println(x);
      lval_del(x);

      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  // Delete the parsers
  mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Lispy);

  return EXIT_SUCCESS;
}
