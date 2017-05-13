#pragma once

enum val_type {
  VAL_TYPE_UNDEFINED,
  VAL_TYPE_BOOL,
  VAL_TYPE_INT,
  VAL_TYPE_DOUBLE,
  VAL_TYPE_STRING,
};

struct val {
  enum val_type type;

  union {
    char b;
    int i;
    double d;
    char *s;
  };
};

#define STACK_SIZE 4096

struct val_stack {
  int top;
  struct val b[STACK_SIZE];
};

void init_val_stack(struct val_stack * s);

struct val *push_val(struct val_stack *s);
struct val *pop_val(struct val_stack *s);

void push_int(struct val_stack *s, int i);

void mult_int(struct val_stack *s);
void less_than_int(struct val_stack *s);

void and_bool(struct val_stack *s);
