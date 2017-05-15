#pragma once

enum modrw_registers {
  MODRW_RAX = 0,
  MODRW_RCX,
  MODRW_RDX,
  MODRW_RBX,
  MODRW_RSP,
  MODRW_RBP,
  MODRW_RSI,
  MODRW_RDI,
};

enum modrw_mod {
  MODRW_INDIRECT = 0,
  MODRW_INDIRECT_PLUS_8,
  MODRW_INDIRECT_PLUS_32,
  MODRW_DIRECT,
};

struct jit_generator {
  unsigned char *code;
  int cursor;
  int size;
};

void init_jit_generator(struct jit_generator* g, int size);
void destroy_jit_generator(struct jit_generator* g);

void generate_return(struct jit_generator *g);
void generate_load_imm64(struct jit_generator *g, enum modrw_registers r, long val);
void generate_call_reg(struct jit_generator *g, enum modrw_registers);
void generate_push_reg(struct jit_generator *g, enum modrw_registers);
void generate_pop_reg(struct jit_generator *g, enum modrw_registers);
void generate_cpy_reg_reg(struct jit_generator *g, enum modrw_registers dst, enum modrw_registers src);

