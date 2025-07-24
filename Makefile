# Makefile for managing the CMake project.
# This file acts as a wrapper for CMake commands.

# The directory where CMake will configure and build the project.
BUILD_DIR = build

# The name of the final executable target as defined in your CMakeLists.txt.
TARGET_NAME = noise_machine

# --- Primary Targets ---

# The default target, executed when you just run `make`.
all: build

# Configure the project (if needed) and compile the code.
build:
	@echo "--- Configuring and Building with CMake ---"
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR)

# Run the executable after ensuring it's built.
run: all
	@echo "--- Running Application ---"
	@$(BUILD_DIR)/$(TARGET_NAME)

# --- Utility Targets ---

# Clean the project by removing the entire build directory.
clean:
	@echo "--- Cleaning Project ---"
	@rm -rf $(BUILD_DIR)

# Force a clean and then rebuild the entire project.
rebuild: clean all

# Phony targets are not files and should always be executed.
.PHONY: all build run clean rebuild
