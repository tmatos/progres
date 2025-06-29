
OBJ_DIR=obj
C_DIR=..

CC=gcc
CXX=g++

CXXFLAGS=`wx-config --cxxflags` -Wall
LFLAGS=`wx-config --libs`

TARGET=ide

_OBJ = IDEMain.o IDEApp.o IDEConfig.o EdicaoDeSinal.o SinaisDrawPane.o util.o
_OBJ += inout.o lex.o mem.o sinais.o strutil.o erros.o
OBJ = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))

default: makedir all

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LFLAGS)

$(OBJ_DIR)/%.o: %.cpp %.h
	$(CXX) -c -o $@ $< -I$(C_DIR) $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(C_DIR)/%.c $(C_DIR)/%.h
	$(CC) -c $< -o $@

.PHONY: all
all: $(TARGET)

.PHONY: makedir
makedir:
	mkdir -p $(OBJ_DIR)

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TARGET)
