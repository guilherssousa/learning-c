#include <stdio.h>
#include <stdlib.h>

#include <editline/history.h>
#include <editline/readline.h>

#include "mpc.h"

typedef struct {
  int type;
  long num;
  int err;
} lval;

// lval types
enum { LVAL_NUM, LVAL_ERR };

// lval error types
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

// create a new number type lval
lval lval_num(long x) {
  lval v;

  v.type = LVAL_NUM;
  v.num = x;

  return v;
}

// create a new error type lval
lval lval_err(int x) {
  lval v;

  v.type = LVAL_ERR;
  v.err = x;

  return v;
}

// Debug function
void lval_print(lval v) {
  switch (v.type) {
  case LVAL_NUM:
    printf("%li", v.num);
    break;
  case LVAL_ERR:
    if (v.err == LERR_DIV_ZERO) {
      printf("Error: Division By Zero!");
    }
    if (v.err == LERR_BAD_OP) {
      printf("Error: Invalid operator!");
    }
    if (v.err == LERR_BAD_NUM) {
      printf("Error: Invalid number!");
    }
    break;
  }
}

// Utility
void lval_println(lval v) {
  lval_print(v);
  putchar('\n');
}

// Check operator and run operation
lval eval_op(lval x, char *op, lval y) {
  if (x.type == LVAL_ERR) {
    return x;
  }
  if (y.type == LVAL_ERR) {
    return y;
  }

  if (strcmp(op, "+") == 0) {
    return lval_num(x.num + y.num);
  }
  if (strcmp(op, "-") == 0) {
    return lval_num(x.num - y.num);
  }
  if (strcmp(op, "*") == 0) {
    return lval_num(x.num * y.num);
  }
  if (strcmp(op, "/") == 0) {
    return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num);
  }
  if (strcmp(op, "%") == 0) {
    return lval_num(x.num % y.num);
  }
  if (strcmp(op, "^") == 0) {
    return lval_num(pow(x.num, y.num));
  }

  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t *t) {
  // If node tag is a number, cast and return
  if (strstr(t->tag, "number")) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }

  // Ignore ( when expr is not number
  char *op = t->children[1]->contents;
  lval x = eval(t->children[2]);

  // Iterate remaining children and combine
  int i = 3;

  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}

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
  mpc_parser_t *Operator = mpc_new("operator");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Lispy = mpc_new("lispy");

  /* define the language syntax */
  mpca_lang(MPCA_LANG_DEFAULT, " number : /-?[0-9]+/ ;              \
    operator : '+' | '-' | '*' | '/' | '%' | '^' ; \
    expr : <number> | '(' <operator> <expr>+ ')' ; \
    lispy : /^/ <operator> <expr>+ /$/ ; \
  ",
            Number, Operator, Expr, Lispy);

  printf("Lispy V0.1\n");
  printf("Press Ctrl+C to Exit\n");

  while (1) {
    /* output message */
    char *input = readline("lispy > ");

    /* Add input to hsitory */
    add_history(input);

    mpc_result_t r;

    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      lval result = eval(r.output);

      lval_println(result);

      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  // Delete the parsers
  mpc_cleanup(4, Number, Operator, Expr, Lispy);

  return EXIT_SUCCESS;
}
