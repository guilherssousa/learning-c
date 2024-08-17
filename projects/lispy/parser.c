#include "mpc.h"

mpc_parser_t *Number;
mpc_parser_t *Symbol;
mpc_parser_t *String;
mpc_parser_t *Comment;
mpc_parser_t *Sexpr;
mpc_parser_t *Qexpr;
mpc_parser_t *Expr;
mpc_parser_t *Lispy;

mpc_parser_t *new_parser() {
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
            | <comment> | <sexpr> | <qexpr> ; \
    lispy  : /^/ <expr>* /$/ ;               \
  ",
            Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, Lispy);

  return Lispy;
}

void clean_parser() {
  mpc_cleanup(8, Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, Lispy);
}
