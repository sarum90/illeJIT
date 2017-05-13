
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include "operations.h"
#include "sample.h"
#include "generate.h"

void printit() {
	printf("Special function\n");
}

void x() {
	printit();
}



int main(int argc, char ** argv) {

  unsigned char code[] = {0xb8, 0x00, 0x00, 0x00, 0x00, 0xc3};
	int num = 9;
  memcpy(&code[1], &num, 4);

  // Allocate writable/executable memory.
  // Note: real programs should not map memory both writable
  // and executable because it is a security risk.
  void *mem = mmap(NULL, 4096, PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	//unsigned char call_code[] = {0xFF, 0x14, 0x25, 0x00, 0x00, 0x00, 0x00, 0xc3};
	unsigned char call_code[] = {0x48, 0xba, 0,0,0,0,0,0,0,0,0xff,0xd2,0xc3};
	long ll = (long)(void *)(&printit);
	memcpy(&call_code[2], &ll, 8);
	for(int i = 0; i < sizeof(call_code); i++) {
		printf("%02x ", call_code[i]);
	}
	printf(" <- try1\n");

  struct jit_generator mg;
  init_jit_generator(&mg, 4096);
  generate_load_imm64(&mg, MODRW_RDX, ll);
  generate_call_reg(&mg, MODRW_RDX);
  generate_return(&mg);
	for(int i = 0; i < sizeof(call_code); i++) {
		printf("%02x ", mg.code[i]);
	}
	printf(" <- try2\n");

  void (*funcg)() = (void (*)())mg.code;
  funcg();

  destroy_jit_generator(&mg);

  return 0;
	printf("%p\n", &printit);
	printf("%p\n", mem);
  memcpy(mem, call_code, sizeof(call_code));

  // The function will return the user's value.
  void (*func)() = (void (*)())mem;
	func();
	//printit();
	//func();
  //printf("HI: %d\n", func());

	printf("%p\n", &push_val);


  struct val_stack *s = malloc(sizeof(struct val_stack));

  sample(s);

  struct val *v = pop_val(s);
  if (v->type != VAL_TYPE_BOOL) {
    printf("Not a bool!\n");
  } else {
    if (v->b) {
      printf("TRUE!\n");
    } else {
      printf("FALSE!\n");
    }
  }
  free(s);
  return 0;
}
