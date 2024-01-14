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

SRCTEST = $(shell find $(TDIR) -name '*.cpp')
OBJTEST = $(patsubst $(TDIR)/%.cpp, $(TDIR)/%.o, $(SRCTEST))

$(TDIR)/%.o: $(TDIR)/%.cpp
	$(CPPC) -c -o $@ $< $(LIBS)

# obs.: main() esta em progres.c
SRC = $(shell find $(SRCDIR) -maxdepth 1 -type f \( -iname '*.c' ! -iname 'progres.c' \))
OBJ = $(patsubst $(SRCDIR)/%.c, $(ODIR)/%.o, $(SRC))

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
