
ARGS=--std=gnu11 -Wall  -Werror -O1

OBJS=out/operations.o out/main.o out/sample.o out/generate.o

default: test
	./test

test: $(OBJS)
	gcc -o test $(OBJS) $(ARGS)

out/generate.o: generate.c generate.h
	@mkdir -p out/
	gcc -o $@ -c $< $(ARGS)

out/operations.o: operations.c operations.h
	@mkdir -p out/
	gcc -o $@ -c $< $(ARGS)

out/sample.o: sample.c sample.h operations.h
	@mkdir -p out/
	gcc -o $@ -c $< $(ARGS)

out/sample.s: sample.c sample.h operations.h
	@mkdir -p out/
	gcc -o $@ -S $< $(ARGS)

out/main.o: main.c operations.h sample.h generate.h
	@mkdir -p out/
	gcc -o $@ -c $< $(ARGS)
