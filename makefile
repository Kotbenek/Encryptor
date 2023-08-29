SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

CC := g++
PARAMETERS := -Wall
OPT_LEVEL := -O3
LIB := 
APP_NAME := encryptor

all: clean build
rebuild: cleaner build

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR):
	@echo Creating $(OBJ_DIR) directory
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@echo Creating $(BIN_DIR) directory
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	@echo Compiling $<
	@$(CC) $< -o $@ -c $(LIB) $(PARAMETERS) $(OPT_LEVEL)

build: $(OBJ_FILES) | $(BIN_DIR)
	@echo Creating bin/$(APP_NAME)
	@$(CC) -o $(BIN_DIR)/$(APP_NAME) $(OBJ_FILES) $(LIB) $(PARAMETERS) $(OPT_LEVEL)

clean:
	@if [ -d ./$(BIN_DIR) ]; then echo Cleaning $(BIN_DIR); rm -f $(BIN_DIR)/*; fi

cleaner: | clean
	@if [ -d ./$(OBJ_DIR) ]; then echo Cleaning $(OBJ_DIR); rm -f $(OBJ_DIR)/*; fi
