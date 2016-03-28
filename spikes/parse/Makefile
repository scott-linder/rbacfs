CC = gcc
CFLAGS = -std=gnu99 -Wall -g

all: main

main: lex.c parse.c def.c main.c

lex.c: lex.l
	flex lex.l

parse.c: parse.y lex.c
	bison parse.y

.PHONY: clean
clean:
	rm -f *.o lex.c lex.h parse.c parse.h main
