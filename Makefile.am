
AUTOMAKE_OPTIONS = foreign

SRC_DIR = src
INC_DIR = .
OBJ_DIR = obj

CC = gcc
CFLAGS = -I$(INC_DIR) -Wall -Wextra

TARGET = progres

SRC = $(shell find $(SRC_DIR) -name '*.c') 

OBJ = $(patsubst $(SRC)/%.c, $(OBJ_DIR)/%.o, $(SRC))

default: makedir all

debug: CFLAGS += -gdwarf-4 -g3
debug: default

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: makedir
makedir:
	mkdir -p $(OBJ_DIR)

.PHONY: all
all: $(TARGET)

.PHONY: clean-local
clean-local:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TARGET)
