# Progres
# Testes unitarios de algumas partes.
# Utilizando CppUnit.

CC = g++
CPPC = g++ 

SRCDIR = ..
IDIR = ..
ODIR = obj
CFLAGS = -I$(IDIR)

LIBS = -lcppunit

SRCSTEST = sinaisTests.cpp
OBJTEST = sinaisTests.o

$(OBJTEST): $(SRCSTEST)
	$(CPPC) -c -o $@ $< $(LIBS)


_DEPS = sinais.h erros.h estruturas.h lex.h eventos.h verilog.h inout.h mem.h simula.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sinais.o erros.o estruturas.o lex.o eventos.o verilog.o inout.o mem.o simula.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

progres: $(OBJ) $(OBJTEST)
	$(CPPC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f ./*.o
	rm -f ./obj/*.o
