CC = g++
CFLAGS = -Wall -std=c++17
SRC_DIR = ./src
SCRIPTS_DIR = ./scripts
BUILD_DIR = ./obj
EXECUTABLE = chess

# List all the source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
SCRIPTS = $(wildcard $(SCRIPTS_DIR)/*.cpp)

# Generate corresponding object file names
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
SCRIPT_OBJECTS = $(patsubst $(SCRIPTS_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SCRIPTS))

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS) $(SCRIPT_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to build object files for script files
$(BUILD_DIR)/%.o: $(SCRIPTS_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to build object files for source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

# Phony target to prevent conflicts with files of the same name
.PHONY: clean
