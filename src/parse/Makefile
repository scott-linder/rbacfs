CFLAGS += -Wno-unused-function

.PHONY: all
all: lib.a

lib.a: lib.o parse.o lex.o def.o
	ar rcs $@ $^

lib.o: lib.c parse.o lex.o def.o

parse.c: parse.y lex.c
	bison parse.y

lex.c: lex.l
	flex lex.l

test: lib.a

.PHONY: clean
clean:
	rm -f *.o lex.c lex.h parse.c parse.h lib.o lib.a
