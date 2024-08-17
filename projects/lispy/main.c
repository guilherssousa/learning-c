#include <stdio.h>
#include <stdlib.h>

#include <editline/history.h>
#include <editline/readline.h>

#include "builtin.h"
#include "lenv.h"
#include "lval.h"
#include "mpc.h"
#include "parser.h"

int main(void) {
  mpc_parser_t *Lispy = new_parser();

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

  clean_parser();

  return EXIT_SUCCESS;
}
