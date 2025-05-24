
OBJ_DIR=obj

CC=gcc
CXX=g++

CXXFLAGS=`wx-config --cxxflags`
LFLAGS=`wx-config --libs`

TARGET=ide

_OBJ = IDEMain.o IDEApp.o IDEConfig.o EdicaoDeSinal.o SinaisDrawPane.o inout.o lex.o memoria.o sinais.o
OBJ = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))

default: makedir all

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LFLAGS)

$(OBJ_DIR)/%.o: %.cpp %.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJ_DIR)/%.o: %.c %.h
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
