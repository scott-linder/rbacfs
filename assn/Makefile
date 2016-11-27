all: pdf

pdf: assignment.pdf

%.pdf: %.tex
	pdflatex $<

.PHONY: clean
clean:
	rm -f *.log
	rm -f *.aux
	rm -f *.out
	rm -f *.pdf
