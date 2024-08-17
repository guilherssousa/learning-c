#include <stdio.h>
#include <stdlib.h>

#include <editline/history.h>
#include <editline/readline.h>

#include "builtin.h"
#include "lenv.h"
#include "lval.h"
#include "mpc.h"
#include "parser.h"

int main(int argc, char *argv[]) {
  parser_init();

  lenv *e = lenv_new();
  lenv_add_builtins(e);

  /* No files supplied, run REPL */
  if (argc == 1) {
    printf("Lispy V0.1\n");
    printf("Press Ctrl+C to Exit\n");

    while (1) {
      /* output message */
      char *input = readline("lispy> ");

      /* Add input to hsitory */
      add_history(input);

      mpc_result_t r;

      if (parser_parse("<stdin>", input, &r)) {
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
  }

  /* Supplied with list of files */
  if (argc >= 2) {
    /* Loop over each supplied filename (starting from 1) */
    for (int i = 1; i < argc; i++) {
      /* Argument list with a single argument, the filename */
      lval *args = lval_add(lval_sexpr(), lval_str(argv[i]));

      /* Pass to builtin load and get the result */
      lval *x = builtin_load(e, args);

      /* If the result is an error, be sure to print it */
      if (x->type == LVAL_ERR)
        lval_println(x);

      lval_del(x);
    }
  }

  lenv_del(e);
  parser_cleanup();

  return EXIT_SUCCESS;
}
