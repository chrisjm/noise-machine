CXX = clang++

# C++17 standard, enable all warnings.
CXXFLAGS = -std=c++17 -Wall -Iinclude

LDFLAGS = -framework CoreAudio -pthread

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/noise_machine
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile source to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: all
	@$(TARGET)

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Project cleaned."

# Phony targets are not files
.PHONY: all run clean
