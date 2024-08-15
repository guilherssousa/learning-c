#ifndef BUILTIN_H
#define BUILTIN_H

#include "lval.h"

lval *builtin(lval *a, char *op);
lval *builtin_op(lval *a, char *op);
lval *builtin_head(lval *a);
lval *builtin_tail(lval *a);
lval *builtin_len(lval *a);
lval *builtin_list(lval *a);
lval *builtin_eval(lval *a);
lval *builtin_join(lval *a);
#endif
