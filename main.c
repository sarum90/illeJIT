
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>

#include "stack.h"
#include "sample.h"
#include "generate.h"
#include "expression.h"

struct timespec get_time() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC_RAW, &t);
  return t;
}

double time_diff_ns(struct timespec t1, struct timespec t2) {
  double ret = t1.tv_nsec - t2.tv_nsec;
  ret += (t1.tv_sec - t2.tv_sec) * 1e9;
  return ret;
}

void printit(int x) {
	printf("Special function: %d\n", x);
}

void call_stack_function(struct jit_generator* g, void * f) {
  generate_cpy_reg_reg(g, MODRW_RDI, MODRW_RBX);
  generate_load_imm64(g, MODRW_RDX, (long)f);
  generate_call_reg(g, MODRW_RDX);
}

void jit_expression_rec(struct jit_generator *g, struct expression * e) {
  if (e->type == EXPR_BINARY_OPERATOR) {
    jit_expression_rec(g, e->arg2);
    jit_expression_rec(g, e->arg1);
    void * lt = (void *)&stack_less_than_int;
    void * mult = (void *)&stack_mult_int;
    void * add = (void *)&stack_add_int;
    void * ab = (void *)&stack_and_bool;
    if (e->binary_op == BIN_OP_MULT_INT) {
      call_stack_function(g, mult);
    } else if (e->binary_op == BIN_OP_ADD_INT) {
      call_stack_function(g, add);
    } else if (e->binary_op == BIN_OP_LESS_THAN_INT) {
      call_stack_function(g, lt);
    } else if (e->binary_op == BIN_OP_AND_BOOL) {
      call_stack_function(g, ab);
    }
  } else if (e->type == EXPR_LITERAL) {
    void * pi = (void *)&push_int;
    if (e->value.type != VAL_TYPE_INT) {
      printf("Non integral literal");
      exit(-1);
    }
    generate_load_imm64(g, MODRW_RSI, e->value.i);
    call_stack_function(g, pi);
  } else {
    void * pvc = (void *)&push_val_copy;
    generate_load_imm64(g, MODRW_RSI, (long)e->variable);
    call_stack_function(g, pvc);
  }
}

void jit_expression(struct jit_generator *g, struct expression * e) {
  // callee save rbx:
  generate_push_reg(g, MODRW_RBX);
  // store a pointer to the stack in rbx:
  generate_cpy_reg_reg(g, MODRW_RBX, MODRW_RDI);

  jit_expression_rec(g, e);

  // restore rbx
  generate_pop_reg(g, MODRW_RBX);
  generate_return(g);
}

struct val evaluate_expression(struct expression *e);

struct val evaluate_bin_op(struct expression *e) {
  struct val l = evaluate_expression(e->arg1);
  struct val r = evaluate_expression(e->arg2);
  switch(e->binary_op) {
    case BIN_OP_MULT_INT:
      return mult_int(&l, &r);
    case BIN_OP_ADD_INT:
      return add_int(&l, &r);
    case BIN_OP_LESS_THAN_INT:
      return less_than_int(&l, &r);
    case BIN_OP_AND_BOOL:
      return and_bool(&l, &r);
    default:
      exit(-1);
      break;
  }
}

struct val evaluate_expression(struct expression *e) {
  switch (e->type) {
    case EXPR_LITERAL:
      return e->value;
    case EXPR_VARIABLE:
      return *e->variable;
    case EXPR_BINARY_OPERATOR:
      return evaluate_bin_op(e);
    default:
      exit(-1);
      break;
  }
}

struct val_stack global_s;

struct val run_jit(struct jit_generator *g) {
  init_val_stack(&global_s);
  void (*funcg)(struct val_stack *s) = (void (*)(struct val_stack *))g->code;
  funcg(&global_s);
  struct val v = *pop_val(&global_s);
  return v;
}

struct val make_int(int i) {
  struct val v;
  v.type = VAL_TYPE_INT;
  v.i = i;
  return v;
}

int main(int argc, char ** argv) {
  struct jit_generator mg;
  init_jit_generator(&mg, 10*4096);
  struct val cv;

  struct expression * e = 
    make_binary_op(BIN_OP_AND_BOOL,
      make_binary_op(BIN_OP_LESS_THAN_INT,
        make_literal(make_int(4)),
        make_binary_op(BIN_OP_MULT_INT,
          make_literal(make_int(2)),
          make_binary_op(BIN_OP_ADD_INT,
            make_variable(&cv),
            make_binary_op(BIN_OP_MULT_INT,
              make_literal(make_int(2)),
              make_variable(&cv)
            )
          )
        )
      ),
      make_binary_op(BIN_OP_LESS_THAN_INT,
        make_literal(make_int(0)),
        make_literal(make_int(1))
      )
    );

  jit_expression(&mg, e);

  for (int i =0; i < 10; i++) {
    cv.type = VAL_TYPE_INT;
    cv.i = i;
    struct val v = run_jit(&mg);
    if (v.type != VAL_TYPE_BOOL) {
      exit(-1);
    }
  }

  struct timespec start_jit = get_time();
  int times_run = 20000000;
  for (int i =0; i < times_run; i++) {
    cv.type = VAL_TYPE_INT;
    cv.i = i;
    struct val v = run_jit(&mg);
    if (v.type != VAL_TYPE_BOOL) {
      exit(-1);
    }
  }
  struct timespec end_jit = get_time();

  for (int i =0; i < 10; i++) {
    cv.type = VAL_TYPE_INT;
    cv.i = i;
    struct val v = evaluate_expression(e);
    if (v.type != VAL_TYPE_BOOL) {
      exit(-1);
    }
  }

  struct timespec start_eval = get_time();
  for (int i =0; i < times_run; i++) {
    cv.type = VAL_TYPE_INT;
    cv.i = i;
    struct val v = evaluate_expression(e);
    if (v.type != VAL_TYPE_BOOL) {
      exit(-1);
    }
  }
  struct timespec end_eval = get_time();


  printf("JIT: %lfns per run\n", time_diff_ns(end_jit, start_jit) / times_run);
  printf("EVAL: %lfns per run\n", time_diff_ns(end_eval, start_eval) / times_run);

  destroy_expression(e);
  destroy_jit_generator(&mg);

  return 0;
}
