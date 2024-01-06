
SRC_DIR = .
INC_DIR = .
OBJ_DIR = obj

CC = gcc
CFLAGS = -I$(INC_DIR) -Wall

TARGET = progres

_DEPS = sinais.h erros.h estruturas.h lex.h eventos.h verilog.h inout.h mem.h simula.h progres.h
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

_OBJ = sinais.o erros.o estruturas.o lex.o eventos.o verilog.o inout.o mem.o simula.o progres.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

default: makedir all

debug: CFLAGS += -gdwarf-4 -g3
debug: default

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: makedir
makedir:
	mkdir -p $(OBJ_DIR)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TARGET)

