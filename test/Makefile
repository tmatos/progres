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

TARGET = progrestest

default: tests

$(OBJTEST): $(SRCSTEST)
	$(CPPC) -c -o $@ $< $(LIBS)

_DEPS = sinais.h erros.h estruturas.h lex.h eventos.h verilog.h inout.h mem.h simula.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sinais.o erros.o estruturas.o lex.o eventos.o verilog.o inout.o mem.o simula.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ) $(OBJTEST)
	$(CPPC) -o $@ $^ $(CFLAGS) $(LIBS)

tests: makedir $(TARGET)
	./$(TARGET)

.PHONY: makedir
makedir:
	mkdir -p $(ODIR)

.PHONY: clean
clean:
	rm -rf $(ODIR)
	rm -f ./*.o
	rm $(TARGET)

