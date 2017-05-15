#include "expression.h"

#include <stdlib.h>

static struct expression *make_expression(enum expr_type t) {
  struct expression* ret = malloc(sizeof(struct expression));
  ret->type = t;
  return ret;
}

struct expression* make_literal(struct val v) {
  struct expression* ret = make_expression(EXPR_LITERAL);
  ret->value = v;
  return ret;
}

struct expression* make_variable(struct val *v) {
  struct expression* ret = make_expression(EXPR_VARIABLE);
  ret->variable = v;
  return ret;
}

struct expression* make_binary_op(
    enum binary_operator o, struct expression *arg1, struct expression *arg2) {
  struct expression* ret = make_expression(EXPR_BINARY_OPERATOR);
  ret->binary_op = o;
  ret->arg1 = arg1;
  ret->arg2 = arg2;
  return ret;
}

void destroy_expression(struct expression *e) {
  if (e->type == EXPR_BINARY_OPERATOR) {
    destroy_expression(e->arg1);
    destroy_expression(e->arg2);
  }
  free(e);
}
