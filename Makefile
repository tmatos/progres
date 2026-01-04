
SRC_DIR = src
INC_DIR = .
OBJ_DIR = obj
WASM_OBJ_DIR = wasm_obj

CC = $(shell command -v emcc || echo "")
CFLAGS = -I$(INC_DIR) -Wall -Wextra

EMCC_FS_FLAGS = \
	-s FORCE_FILESYSTEM=1 \
	-lidbfs.js \
	-s EXPORTED_RUNTIME_METHODS=['FS']

EMCC_EXPORT_FLAGS = \
	-s EXPORTED_FUNCTIONS='["_w_load_module", "_main"]' \
	-s EXPORTED_RUNTIME_METHODS='["ccall", "callMain"]'

SRC = $(shell find $(SRC_DIR) -name '*.c')
OBJ = $(patsubst $(SRC)/%.c, $(OBJ_DIR)/%.o, $(SRC))
WASM_OBJ = $(patsubst $(OBJ)/%,$(WASM_OBJ_DIR)/%,$(OBJ))

WEB_TARGET = webprogres

ifeq (, $(CC))
$(error "Emscripten was not found! Install or activate it to build the WebAssembly target.")
else
$(info ---)
$(shell $(CC) -v)
$(info ---)
endif

default: all

.PHONY: wasm
wasm: makewebdir $(WEB_TARGET)

$(WASM_OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h 
	$(CC) -c -o $@ $< -s "EXPORTED_RUNTIME_METHODS=ccall"

$(WEB_TARGET): $(WASM_OBJ)
	$(CC) $^ -o $@.js $(CFLAGS) $(EMCC_FS_FLAGS) $(EMCC_EXPORT_FLAGS)
	rm -f web_editor/$(WEB_TARGET).js
	rm -f web_editor/$(WEB_TARGET).wasm
	cp $(WEB_TARGET).js web_editor/
	cp $(WEB_TARGET).wasm web_editor/

.PHONY: makewebdir
makewebdir:
	mkdir -p $(WASM_OBJ_DIR)

.PHONY: all
all: wasm 

.PHONY: clean
clean:
	rm -f $(WASM_OBJ_DIR)/*.o
	rm -df $(WASM_OBJ_DIR)
	rm -f $(WEB_TARGET).js
	rm -f $(WEB_TARGET).wasm
