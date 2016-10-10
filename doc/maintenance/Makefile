all: pdf

pdf: maintenance.pdf

%.pdf: %.tex modules.png
	pdflatex $<

modules.png: modules.dot
	dot -Tpng $< >$@

.PHONY: clean
clean:
	rm -f *.log
	rm -f *.aux
	rm -f *.out
	rm -f *.pdf
	rm -f *.png
