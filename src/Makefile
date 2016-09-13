CC = gcc
CFLAGS = -std=gnu99 -Wall
LDFLAGS = -lfuse
MODS = parse fuse hashmap list policy

export CC
export CFLAGS

rbacfs: rbacfs.c $(addsuffix /lib.a,${MODS}) $(LDFLAGS)

%/lib.a: FORCE
	cd $*; make

.PHONY: clean
clean:
	for mod in ${MODS}; do cd $$mod; make clean; cd ..; done
	rm -f rbacfs

# Used to make patterned rules (like %/lib.a) phony, so they always build.
# This is required because each module is treated as a "subproject", having its
# own makefile. To ensure our master binary does not need to be rebuilt, we
# need to manually confirm no submodule needs to be rebuilt.
.PHONY: FORCE
FORCE:
