#include "sample.h"

void  sample(struct val_stack *s) {
  push_int(s, 1);
  push_int(s, 0);
  stack_less_than_int(s);
  push_int(s, 3);
  push_int(s, 2);
  stack_mult_int(s);
  push_int(s, 4);
  stack_less_than_int(s);
  stack_and_bool(s);
}
