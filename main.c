
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
	long ll = (long)(void *)(&printit);
  struct jit_generator mg;
  init_jit_generator(&mg, 4096);
  generate_load_imm64(&mg, MODRW_RDX, ll);
  generate_call_reg(&mg, MODRW_RDX);
  generate_return(&mg);
	for(int i = 0; i < 13; i++) {
		printf("%02x ", mg.code[i]);
	}
	printf(" <- try2\n");

  void (*funcg)() = (void (*)())mg.code;
  funcg();

  destroy_jit_generator(&mg);

  return 0;
}
