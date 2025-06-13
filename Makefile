
SRC_DIR = .
INC_DIR = .
OBJ_DIR = obj

CC = gcc
CFLAGS = -I$(INC_DIR) -Wall

TARGET = progres

SRC  = strutil.c sinais.c erros.c estruturas.c lex.c preprocessor.c
SRC += eventos.c verilog.c inout.c mem.c simula.c progres.c

DEPS = $(patsubst %.c, $(SRC_DIR)/%.h, $(SRC))

OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))

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
