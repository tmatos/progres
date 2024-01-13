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

_OBJTEST = estruturasTests.o sinaisTests.o lexTests.o eventosTests.o main.o
OBJTEST = $(patsubst %,$(TDIR)/%,$(_OBJTEST))

$(TDIR)/%.o: $(TDIR)/%.cpp
	$(CPPC) -c -o $@ $< $(LIBS)

_OBJ = sinais.o erros.o estruturas.o lex.o eventos.o verilog.o inout.o mem.o simula.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.c $(IDIR)/%.h
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
