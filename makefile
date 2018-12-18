# Muchas gracias a Hilton Lipschitz por este makefile. Se ecuentra a
# https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
CC = g++
CFLAGS = -std=c++17

SRCDIR   := src
BUILDDIR := build
TARGET   := bin/main

SRCEXT   := cpp
#SOURCES  := $(shell find $(SRCDIR) -type f -name \*.$(SRCEXT))
SOURCES  := $(shell find $(SRCDIR) -type f -name \*.$(SRCEXT) ! -name *ext*.cpp)
OBJECTS  := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC      := -I include

DOCSDIR := docs
TEXSRCS := $(shell find $(DOCSDIR) -maxdepth 2 -type f -name \*.tex)
TEXBUILDDIRS = $(shell find $(DOCSDIR) -type d -name build)
PDFDOCS = $(shell find $(TEXBUILDDIRS) -type f -name \*.pdf)
PDFDOCSDIR := docs/pdf

RM = rm -rf

all:

docs: $(TEXSRCS)
	@latexmk -pdf -use-make- -pdflatex="pdflatex -interaction=nonstopmode" \
		     -outdir=build -cd $(TEXSRCS)
	@make movepdfs
	#@$(RM) $(TEXBUILDDIRS)

presentation: docs/final/rkev_presentation-jackson_cole.tex
	@latexmk -pdf -use-make- -pdflatex="pdflatex"\
		-outdir=build -cd $<
	@make movepdfs


movepdfs: $(PDFDOCS)
	@mkdir -p $(PDFDOCSDIR)
	@mv $< $(PDFDOCSDIR)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
	@$(RM) $(shell find ./python/ -name \*.pyc)

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester
main: $(TARGET)
	@echo "$(CC) $(CFLAGS) -o $(TARGET) $^"
