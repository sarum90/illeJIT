
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include "stack.h"
#include "sample.h"
#include "generate.h"
#include "expression.h"

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
    void * ab = (void *)&stack_and_bool;
    if (e->binary_op == BIN_OP_MULT_INT) {
      call_stack_function(g, mult);
    } else if (e->binary_op == BIN_OP_LESS_THAN_INT) {
      call_stack_function(g, lt);
    } else if (e->binary_op == BIN_OP_AND_BOOL) {
      call_stack_function(g, ab);
    }
  } else {
    void * pvc = (void *)&push_val_copy;
    struct val *v;
    if (e->type == EXPR_LITERAL) {
      v = &e->value;
    } else {
      v = e->variable;
    }
    generate_load_imm64(g, MODRW_RSI, (long)v);
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

  void * pi = (void *)&push_int;
  void * pvc = (void *)&push_val_copy;
  void * lt = (void *)&stack_less_than_int;
  void * mult = (void *)&stack_mult_int;
  void * ab = (void *)&stack_and_bool;

  struct expression * e = 
    make_binary_op(BIN_OP_AND_BOOL,
      make_binary_op(BIN_OP_LESS_THAN_INT,
        make_literal(make_int(4)),
        make_binary_op(BIN_OP_MULT_INT,
          make_literal(make_int(2)),
          make_variable(&cv)
        )
      ),
      make_binary_op(BIN_OP_LESS_THAN_INT,
        make_literal(make_int(0)),
        make_literal(make_int(1))
      )
    );

  if (0) {
    // callee save rbx:
    generate_push_reg(&mg, MODRW_RBX);
    // store a pointer to the stack in rbx:
    generate_cpy_reg_reg(&mg, MODRW_RBX, MODRW_RDI);

    generate_load_imm64(&mg, MODRW_RSI, 1);
    call_stack_function(&mg, pi);

    generate_load_imm64(&mg, MODRW_RSI, 0);
    call_stack_function(&mg, pi);

    call_stack_function(&mg, lt);

    generate_load_imm64(&mg, MODRW_RSI, (long)&cv);
    call_stack_function(&mg, pvc);

    generate_load_imm64(&mg, MODRW_RSI, 2);
    call_stack_function(&mg, pi);

    call_stack_function(&mg, mult);

    generate_load_imm64(&mg, MODRW_RSI, 4);
    call_stack_function(&mg, pi);

    call_stack_function(&mg, lt);
    call_stack_function(&mg, ab);

    // restore rbx
    generate_pop_reg(&mg, MODRW_RBX);
    generate_return(&mg);
  } else {
    jit_expression(&mg, e);
  }

	for(int i = 0; i < 40; i++) {
		printf("%02x ", mg.code[i]);
	}
	printf(" <- try2\n");

  struct val_stack s;
  init_val_stack(&s);
  void (*funcg)() = (void (*)(struct val_stack *))mg.code;
  for (int i = 1; i < 5; i++) {
    cv.type = VAL_TYPE_INT;
    cv.i = i;
    funcg(&s);
    struct val *v = pop_val(&s);
    if (v->type != VAL_TYPE_BOOL) {
      printf("Not a bool!\n");
    } else {
      printf("%s\n", v->b ? "true" : "false");
    }
  }
  cv.type = VAL_TYPE_BOOL;
  cv.b = 1;
  funcg(&s);
  struct val *v = pop_val(&s);
  if (v->type != VAL_TYPE_BOOL) {
    printf("Not a bool!\n");
  } else {
    printf("%s\n", v->b ? "true" : "false");
  }

  destroy_expression(e);
  destroy_jit_generator(&mg);

  return 0;
}
