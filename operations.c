#include <assert.h>
#include <stdio.h>

#include "operations.h"


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

void mult_int(struct val_stack *s) {
  struct val *l = pop_val(s);
  struct val *r = pop_val(s);
  struct val *res = push_val(s);
  if(l->type != VAL_TYPE_INT ||
     r->type != VAL_TYPE_INT) {
    res->type = VAL_TYPE_UNDEFINED;
  } else {
    res->type = VAL_TYPE_INT;
    res->i = l->i * r->i;
  }
}

void less_than_int(struct val_stack *s) {
  struct val *l = pop_val(s);
  struct val *r = pop_val(s);
  struct val *res = push_val(s);
  if(l->type != VAL_TYPE_INT ||
     r->type != VAL_TYPE_INT) {
    res->type = VAL_TYPE_BOOL;
    res->b = 0;
  } else {
    res->type = VAL_TYPE_BOOL;
    res->b = (l->i < r->i);
  }
}

void and_bool(struct val_stack *s) {
  struct val *l = pop_val(s);
  struct val *r = pop_val(s);
  struct val *res = push_val(s);
  if(l->type != VAL_TYPE_BOOL ||
     r->type != VAL_TYPE_BOOL) {
    res->type = VAL_TYPE_BOOL;
    res->b = 0;
  } else {
    res->type = VAL_TYPE_BOOL;
    res->b = (l->b && r->b);
  }
}
