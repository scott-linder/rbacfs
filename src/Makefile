CC = gcc
CFLAGS = -std=gnu99 -Wall
LDFLAGS = -lfuse
# Default installation prefix
PREFIX ?= /usr/local
# List of all module directories
MODS = parse fuse hashmap list policy
# List of all module libraries
LIBS = $(addsuffix /lib.a,${MODS})

# Ensure the defaults for these are exported to sub-makefiles
export CC
export CFLAGS

# The main executable depends on each module.
rbacfs: rbacfs.c $(LIBS) $(LDFLAGS)

.PHONY: test
test: $(LIBS)
	@for mod in ${MODS}; \
		do cd $$mod; \
		make test \
			&& ./test \
			&& echo -e "\e[0;32m" $$mod "passed" "\e[0m" \
			|| echo -e "\e[0;31m" $$mod "failed" "\e[0m"; \
		cd ..; \
	done

# We cannot know whether each module need to be rebuilt, so we ensure we always
# force them to check if their lib.a is required.
%/lib.a: FORCE
	cd $*; make

# Install to /usr/local/bin
.PHONY: install
install:
	install -m 0755 rbacfs ${PREFIX}/bin

# We delgate cleaning to each submodule.
.PHONY: clean
clean:
	for mod in ${MODS}; do cd $$mod; make clean; cd ..; done
	rm -f rbacfs

# Used to make patterned rules (like %/lib.a) phony, so they always build.
.PHONY: FORCE
FORCE:
