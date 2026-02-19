# App config
APP_NAME := wisp
CXX := g++
CXX_STANDARD := -std=c++20
CXX_FLAGS := -Wall -Wextra

# Directories
PROJECT_ROOT := $(abspath .)
SOURCE_DIR := $(PROJECT_ROOT)/Source
BUILD_DIR := $(PROJECT_ROOT)/build

# Sources
SOURCES := \
	$(SOURCE_DIR)/main.cpp \
	$(SOURCE_DIR)/lexer/lexer.cpp \
	$(SOURCE_DIR)/arguments/arg.cpp

# Output
TARGET := $(BUILD_DIR)/$(APP_NAME)

# Default target
all: $(TARGET)

# Build rule
$(TARGET): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	@echo "Building $(APP_NAME)..."
	@echo "Source dir: $(SOURCE_DIR)"
	@echo "Build dir: $(BUILD_DIR)"
	$(CXX) $(CXX_STANDARD) $(CXX_FLAGS) $(SOURCES) -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# Clean rule
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
