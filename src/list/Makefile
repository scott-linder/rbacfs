
.PHONY: all
all: lib.a

lib.a: lib.o
	ar rcs $@ $^

.PHONY: clean
clean:
	rm -rf lib.a lib.o
