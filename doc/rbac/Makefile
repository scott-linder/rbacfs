all: rbac.png flat.png hier.png hiertree.png

%.png: %.dot
	dot -Gdpi=140 -Tpng $< >$@

.PHONY: clean
clean:
	rm -f *.png
