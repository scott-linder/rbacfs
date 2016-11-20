CC = gcc
CFLAGS += -D_FILE_OFFSET_BITS=64

.PHONY: all
all: lib.a

lib.a: lib.o
	ar rcs $@ $^

.PHONY: clean
clean:
	rm -f lib.a lib.o
