SRC_DIR := src
OBJ_DIR := obj
DEP_DIR := dep
BIN_DIR := bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(DEP_DIR)/%.d,$(SRC_FILES))
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

CXX := g++
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

$(DEP_DIR):
	@echo Creating $(DEP_DIR) directory
	@mkdir -p $(DEP_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d | $(DEP_DIR)
	@echo Compiling $<
	@$(CXX) $< -o $@ -c $(LIB) $(PARAMETERS) $(OPT_LEVEL) $(DEP_FLAGS)

build: $(OBJ_FILES) | $(BIN_DIR)
	@echo Creating bin/$(APP_NAME)
	@$(CXX) -o $(BIN_DIR)/$(APP_NAME) $(OBJ_FILES) $(LIB) $(PARAMETERS) $(OPT_LEVEL)

clean:
	@if [ -d ./$(BIN_DIR) ]; then echo Cleaning $(BIN_DIR); rm -f $(BIN_DIR)/*; fi

cleaner: | clean
	@if [ -d ./$(OBJ_DIR) ]; then echo Cleaning $(OBJ_DIR); rm -f $(OBJ_DIR)/*; fi
	@if [ -d ./$(DEP_DIR) ]; then echo Cleaning $(DEP_DIR); rm -f $(DEP_DIR)/*; fi

$(DEP_FILES):
include $(wildcard $(DEP_FILES))
