# Progres
# Testes unitarios utilizando CppUnit
# Sumario de cobertura com `lcov`

CC = g++
CPPC = g++ 

SRCDIR = ..
IDIR = ..
ODIR = .
TDIR = .

CFLAGS = -I$(IDIR) --coverage -g -O0
LIBS = -lcppunit

TARGET = progrestest
COV_FILE = coverage.txt

default: tests

_SRCSTEST = sinaisTests.cpp lexTests.cpp main.cpp
SRCTEST = $(patsubst %,$(TDIR)/%,$(_SRCTEST))
_OBJTEST = sinaisTests.o lexTests.o main.o
OBJTEST = $(patsubst %,$(TDIR)/%,$(_OBJTEST))

$(TDIR)/%.o: $(TDIR)/%.cpp
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

coverage: tests
	lcov -q -d . -c -o $(COV_FILE) && lcov --summary $(COV_FILE)

.PHONY: makedir
makedir:
	mkdir -p $(ODIR)

.PHONY: clean
clean:
	if [ $(ODIR) != . ]; then rm -rf $(ODIR); fi
	rm -f ./*.o
	rm -f ./*.gcno
	rm -f ./*.gcda
	rm -f $(COV_FILE)
	rm -f $(TARGET)
