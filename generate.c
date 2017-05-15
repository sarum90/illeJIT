#include "generate.h"

#include <assert.h>
#include <stdlib.h>
#include <sys/mman.h>

struct modrw {
  union {
    struct {
      char r_m :3;
      char opcode :3;
      char mod :2;
    };
    char as_char;
  };
};
typedef char assert_size_of_modrw[sizeof(struct modrw) == 1 ? 1: -1];

struct unary_opcode {
  union {
    struct {
      char r_m :3;
      char opcode :5;
    };
    char as_char;
  };
};
typedef char assert_size_of_movimm[sizeof(struct unary_opcode) == 1 ? 1: -1];

void init_jit_generator(struct jit_generator* g, int size) {
  g->code = mmap(NULL, size, PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  assert(g->code); // Failed to mmap.
  g->cursor = 0;
  g->size = size;
}

void destroy_jit_generator(struct jit_generator* g) {
  munmap(g->code, g->size);
  g->code = NULL;
  g->cursor = 0;
  g->size = 0;
}

static unsigned char * generate_bytes(struct jit_generator *g, int bytes) {
  unsigned char * ret = &g->code[g->cursor];
  g->cursor += bytes;
  assert(g->cursor < g->size); // generated code too large;
  return ret;
}

void generate_return(struct jit_generator *g) {
  generate_bytes(g, 1)[0] = 0xc3;
}

void generate_load_imm64(struct jit_generator *g, enum modrw_registers r, long val) {
  generate_bytes(g, 1)[0] = 0x48;
  struct unary_opcode m;
  m.opcode = 0x17;
  m.r_m = r;
  generate_bytes(g, 1)[0] = m.as_char;
  *((long *)generate_bytes(g, 8)) = val;
}

void generate_call_reg(struct jit_generator *g, enum modrw_registers r) {
  generate_bytes(g, 1)[0] = 0xff;
  struct modrw mrw;
  mrw.mod = MODRW_DIRECT;
  mrw.opcode = 2;
  mrw.r_m = r;
  generate_bytes(g, 1)[0] = mrw.as_char;
}

void generate_push_reg(struct jit_generator *g, enum modrw_registers r) {
  struct unary_opcode m;
  m.opcode = 0x0a;
  m.r_m = r;
  generate_bytes(g, 1)[0] = m.as_char;
}

void generate_pop_reg(struct jit_generator *g, enum modrw_registers r) {
  struct unary_opcode m;
  m.opcode = 0x0b;
  m.r_m = r;
  generate_bytes(g, 1)[0] = m.as_char;
}

void generate_cpy_reg_reg(struct jit_generator *g, enum modrw_registers dst, enum modrw_registers src)
{
  // rex.w (64 bit)
  generate_bytes(g, 1)[0] = 0x48;
  // mov
  generate_bytes(g, 1)[0] = 0x89;

  struct modrw mrw;
  mrw.mod = MODRW_DIRECT;
  mrw.opcode = src;
  mrw.r_m = dst;
  generate_bytes(g, 1)[0] = mrw.as_char;
}

