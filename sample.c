#include "sample.h"

void  sample(struct val_stack *s) {
  push_int(s, 1);
  push_int(s, 0);
  less_than_int(s);
  push_int(s, 3);
  push_int(s, 2);
  mult_int(s);
  push_int(s, 4);
  less_than_int(s);
  and_bool(s);
}
