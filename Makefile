# GNU C++ compiler and archiver; command-line assignments can override either tool.
CXX := g++
AR := ar
ARFLAGS := rcs

# Release builds are used unless the caller requests CONFIG=debug.
CONFIG ?= release
# Compile up to four independent source files concurrently by default.
JOBS ?= 4

# Recursively find matching files in this directory and all nested directories.
rwildcard = $(wildcard $(1)$(2)) $(foreach dir,$(wildcard $(1)*),$(if $(wildcard $(dir)/.),$(call rwildcard,$(dir)/,$(2))))

# The test file has its own main function, so exclude it from the library sources.
SOURCES := $(call rwildcard,src/,*.cpp)
HEADERS := $(call rwildcard,include/,*.hpp)
BUILD_DIR := build/$(CONFIG)
TARGET := $(BUILD_DIR)/libgeom3.a
OBJECTS := $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJECT := $(BUILD_DIR)/tests/test.o
TEST_TARGET := $(BUILD_DIR)/geometry_tests.exe

# Search the public include root for headers such as <geom3/geom3.hpp>.
CPPFLAGS := -Iinclude

# Keep `make` building the library even though helper targets are declared below.
.DEFAULT_GOAL := all

# MSYS2 provides a POSIX shell even on Windows, while native Windows Make uses cmd.exe.
ifeq ($(OS),Windows_NT)
ifneq ($(MSYSTEM),)
  MKDIR = mkdir -p $(dir $@)
  REMOVE = rm -rf build
else
  MKDIR = if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
  REMOVE = if exist "build" rmdir /s /q "build"
endif
else
  MKDIR = mkdir -p $(dir $@)
  REMOVE = rm -rf build
endif

# Debug builds keep symbols and disable optimizations; release builds optimize code.
ifeq ($(CONFIG),debug)
  CXXFLAGS := -std=c++20 -Wall -Wextra -O0 -g
else
  CXXFLAGS := -std=c++20 -Wall -Wextra -O2
endif

.PHONY: all clean test

# Build independent translation units concurrently, then archive them into the library.
all:
	$(MAKE) -j$(JOBS) _build

.PHONY: _build
_build: $(TARGET)

# Archive all compiled implementation files into a static library.
$(TARGET): $(OBJECTS)
	$(MKDIR)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

# Compile each C++ source into its matching object file; any header change rebuilds it.
$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	$(MKDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Compile the geometry test program separately from the library implementation.
$(TEST_OBJECT): tests/test.cpp $(HEADERS)
	$(MKDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Link the tests against the static library.
$(TEST_TARGET): $(TEST_OBJECT) $(TARGET)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECT) $(TARGET) -o $@

# Build independent files concurrently, then run the geometry tests.
test:
	$(MAKE) -j$(JOBS) _test

.PHONY: _test
_test: $(TEST_TARGET)
	$(TEST_TARGET)

# Remove only generated build products.
clean:
	$(REMOVE)
