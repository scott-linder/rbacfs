all: rbac.png

rbac.png: rbac.dot
	dot -Tpng $< >$@

.PHONY: clean
clean:
	rm -f *.png
