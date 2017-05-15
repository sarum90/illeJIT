#pragma once

#include "operations.h"

#define STACK_SIZE 4096

struct val_stack {
  int top;
  struct val b[STACK_SIZE];
};

void init_val_stack(struct val_stack * s);

struct val *push_val(struct val_stack *s);
struct val *pop_val(struct val_stack *s);

void push_int(struct val_stack *s, int i);
void push_val_copy(struct val_stack *s, struct val* v);

void stack_mult_int(struct val_stack *s);
void stack_add_int(struct val_stack *s);
void stack_less_than_int(struct val_stack *s);

void stack_and_bool(struct val_stack *s);
