
.PHONY: all
all: lib.a

lib.a: lib.o
	ar rcs $@ $^

test: lib.a

.PHONY: clean
clean:
	rm -rf lib.a lib.o test
