@echo off
pushd thesis

if not exist build mkdir build

md2tex abstract.md build\abstract.tex
md2tex intro.md build\intro.tex
md2tex chapter1.md build\chapter1.tex
md2tex chapter2.md build\chapter2.tex
md2tex chapter3.md build\chapter3.tex
md2tex summary.md build\summary.tex

copy .\refs.bib .\build\refs.bib

pdflatex -aux-directory=build -quiet thesis.tex
pushd build
bibtex -quiet thesis
popd build
pdflatex -aux-directory=build -quiet thesis.tex
pdflatex -aux-directory=build -quiet thesis.tex

start thesis.pdf

popd
