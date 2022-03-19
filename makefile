ELF1 = a.out
ELF2 = a.noelide.out
LIB  = 

SDIR = src
IDIR = include
ODIR = obj
BDIR = bin

SRC  = main.cpp baz.cpp foo.cpp
CSRC = mem_align.c
DEP  = baz.hpp foo.hpp template.hpp yoda.hpp mem_align.h

$(ELF1): $(patsubst %,$(ODIR)/%.20.o,$(basename $(SRC))) \
	$(patsubst %,$(ODIR)/%.cc.o,$(basename $(CSRC)))
	mkdir -p $(BDIR)
	g++ $^ $(patsubst %,-l%,$(LIB)) -o $(BDIR)/$@

$(ELF2): $(patsubst %,$(ODIR)/%.14.o,$(basename $(SRC))) \
	$(patsubst %,$(ODIR)/%.cc.o,$(basename $(CSRC)))
	mkdir -p $(BDIR)
	g++ $^ $(patsubst %,-l%,$(LIB)) -o $(BDIR)/$@

$(ODIR)/%.20.o: $(SDIR)/%.cpp $(patsubst %,$(IDIR)/%,$(DEP))
	g++ -Wall -std=c++20 -O0 -g -I$(IDIR) -DCPP20 -c $< -o $@

$(ODIR)/%.14.o: $(SDIR)/%.cpp $(patsubst %,$(IDIR)/%,$(DEP))
	g++ -Wall -std=c++14 -O0 -g -fno-elide-constructors -I$(IDIR) -c $< -o $@

$(ODIR)/%.cc.o: $(SDIR)/%.c $(patsubst %,$(IDIR)/%,$(DEP))
	gcc -Wall -O0 -g -I$(IDIR) -c $< -o $@

all: options $(ELF1) $(ELF2) 

clean:
	rm -rf $(ODIR)/*.o $(BDIR)/

options:
	@echo "ELF1 = ${ELF1}"
	@echo "ELF2 = ${ELF2}" 
	@echo "LIB  = ${LIB}"
	@echo "SDIR = ${SDIR}" 
	@echo "IDIR = ${IDIR}" 
	@echo "ODIR = ${ODIR}" 
	@echo "BDIR = ${BDIR}" 
	@echo "SRC  = ${SRC}"
	@echo "CSRC  = ${CSRC}"
	@echo "DEP  = ${DEP}"

.PHONY: all clean options
