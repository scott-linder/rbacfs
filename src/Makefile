CC = gcc
CFLAGS = -std=gnu99 -Wall
LDFLAGS = -lfuse
MODS = parse fuse

export CC
export CFLAGS

rbacfs: rbacfs.c parse/lib.a fuse/lib.a

%/lib.a:
	cd $*; make

.PHONY: clean
clean:
	for mod in ${MODS}; do cd $$mod; make clean; cd ..; done
	rm -f rbacfs
