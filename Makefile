# Makefile for Chess project

CC = g++
CFLAGS = -Wall -std=c++17
SRC_DIR = ./src
BUILD_DIR = ./obj
EXECUTABLE = chess

# List all the source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding object file names
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

# Phony target to prevent conflicts with files of the same name
.PHONY: clean
