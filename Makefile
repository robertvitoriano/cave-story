# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -g -std=c++17 -I source/headers -I /usr/include $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_mixer SDL2_ttf)
LDFLAGS := $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer SDL2_ttf) 

# Directories
SRC_DIR := source/src
HEADER_DIR := source/headers
OBJ_DIR := build
BIN_DIR := bin

# Output binary name
TARGET := $(BIN_DIR)/game

# Source and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

rebuild-level:
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/level.cpp -o $(OBJ_DIR)/level.o
	@echo "level.cpp has been recompiled."

# Default target
all: $(TARGET)

# Create target binary
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the game
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run
