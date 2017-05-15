#include <assert.h>
#include <stdio.h>

#include "stack.h"


void init_val_stack(struct val_stack * s) {
  s->top = 0;
  return;
}

void pp(struct val *v) {
  if (v->type == VAL_TYPE_INT) {
    printf("val i: %d\n", v->i);
  } else if (v->type == VAL_TYPE_BOOL) {
    printf("val: %s\n", v->b ? "true": "false");
  } else {
    printf("unknown\n");
  }
}

struct val *push_val(struct val_stack *s) {
  s->top++;
  assert(s->top < STACK_SIZE); // "blew out the value stack"
  return &s->b[s->top - 1];
}

struct val *pop_val(struct val_stack *s) {
  assert(s->top > 0); // "popped off of an empty stack"
  s->top--;
  struct val * v = &s->b[s->top];
  return v;
}

void push_int(struct val_stack *s, int i) {
  struct val *v = push_val(s);
  v->type = VAL_TYPE_INT;
  v->i = i;
}

void push_val_copy(struct val_stack *s, struct val* v) {
  struct val *sv = push_val(s);
  *sv = *v;
}

void stack_mult_int(struct val_stack *s) {
  struct val *l = pop_val(s);
  struct val *r = pop_val(s);
  *push_val(s) = mult_int(l, r);
}

void stack_add_int(struct val_stack *s) {
  struct val *l = pop_val(s);
  struct val *r = pop_val(s);
  *push_val(s) = add_int(l, r);
}

void stack_less_than_int(struct val_stack *s) {
  struct val *l = pop_val(s);
  struct val *r = pop_val(s);
  *push_val(s) = less_than_int(l, r);
}

void stack_and_bool(struct val_stack *s) {
  struct val *l = pop_val(s);
  struct val *r = pop_val(s);
  *push_val(s) = and_bool(l, r);
}
