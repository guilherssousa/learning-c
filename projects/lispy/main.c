#include <stdio.h>
#include <stdlib.h>

#include <editline/history.h>
#include <editline/readline.h>

#include "builtin.h"
#include "lenv.h"
#include "lval.h"
#include "mpc.h"

int main(void) {
  // Parsers definition
  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Symbol = mpc_new("symbol");
  mpc_parser_t *String = mpc_new("string");
  mpc_parser_t *Comment = mpc_new("comment");
  mpc_parser_t *Sexpr = mpc_new("sexpr");
  mpc_parser_t *Qexpr = mpc_new("qexpr");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Lispy = mpc_new("lispy");

  /* define the language syntax */

  mpca_lang(MPCA_LANG_DEFAULT, "             \
    number : /-?[0-9]+/ ;                    \
    symbol : /[a-zA-Z0-9_+%\\-***\\/\\\\=<>!&]+/ ;\
    string  : /\"(\\\\.|[^\"])*\"/ ;         \
    comment : /;[^\\r\\n]*/ ;                \
    sexpr  : '(' <expr>* ')' ;               \
    qexpr  : '{' <expr>* '}' ;               \
    expr   : <number> | <symbol> | <string>  \
            | <comment > |  <sexpr> | <qexpr> ; \
    lispy  : /^/ <expr>* /$/ ;               \
  ",
            Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, Lispy);

  printf("Lispy V0.1\n");
  printf("Press Ctrl+C to Exit\n");

  lenv *e = lenv_new();
  lenv_add_builtins(e);

  while (1) {
    /* output message */
    char *input = readline("lispy > ");

    /* Add input to hsitory */
    add_history(input);

    mpc_result_t r;

    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      lval *x = lval_eval(e, lval_read(r.output));
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
  mpc_cleanup(8, Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, Lispy);

  return EXIT_SUCCESS;
}
