#pragma once

#include "operations.h"

enum expr_type {
  EXPR_LITERAL,
  EXPR_VARIABLE,
  EXPR_BINARY_OPERATOR,
};

enum binary_operator {
  BIN_OP_MULT_INT,
  BIN_OP_ADD_INT,
  BIN_OP_LESS_THAN_INT,
  BIN_OP_AND_BOOL,
};

struct expression {
  enum expr_type type;
  union {
    struct val value;
    struct val *variable;
    struct {
      enum binary_operator binary_op;
      struct expression* arg1;
      struct expression* arg2;
    };
  };
};

struct expression* make_literal(struct val v);
struct expression* make_variable(struct val *v);
struct expression* make_binary_op(
    enum binary_operator o, struct expression *arg1, struct expression *arg2);
void destroy_expression(struct expression *e);

