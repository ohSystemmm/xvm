# xvm C++ build - ohSystemmm 2026

PROJECT_NAME = xvm
CONFIG = debug
CXX ?= g++
BUILD_ROOT = build
BUILD_DIR := $(BUILD_ROOT)/$(CONFIG)
OBJ_DIR := $(BUILD_DIR)/obj

EXE_EXT = .exe
RUN_PREFIX :=
MKDIR_P = if not exist "$@" mkdir "$@"
REMOVE_DIR = if exist "$(1)" rmdir /s /q "$(1)"

ifeq ($(filter $(CONFIG),debug release),)
$(error CONFIG must be either 'debug' or 'release' (got '$(CONFIG)'))
endif

SOURCES := $(wildcard src/*.cpp)
ifeq ($(strip $(SOURCES)),)
$(error No C++ source files found in src/)
endif

OBJECTS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)
TARGET = $(BUILD_DIR)/$(PROJECT_NAME)$(EXE_EXT)

CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic
ifeq ($(CONFIG),debug)
CXXFLAGS += -O0 -g
else
CXXFLAGS += -O2 -DNDEBUG
endif

.DEFAULT_GOAL := build
.PHONY: build run debug release clean info help

build: $(TARGET)
	@echo Built $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o "$@"

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -MF "$(@:.o=.d)" -c "$<" -o "$@"

$(BUILD_DIR):
	$(MKDIR_P)

$(OBJ_DIR): | $(BUILD_DIR)
	$(MKDIR_P)

run: $(TARGET)
	$(RUN_PREFIX)"$(TARGET)"

debug:
	$(MAKE) CONFIG=debug build

release:
	$(MAKE) CONFIG=release build

clean:
	$(call REMOVE_DIR,$(BUILD_ROOT))

info:
	@echo Project:    $(PROJECT_NAME)
	@echo Compiler:   $(CXX)
	@echo Config:     $(CONFIG)
	@echo Target:     $(TARGET)
	@echo Sources:    $(SOURCES)

help:
	@echo "xvm build targets:"
	@echo "  build            Build the selected configuration (default: debug)"
	@echo "  run              Build and run the selected configuration"
	@echo "  debug            Build with debug symbols and no optimization"
	@echo "  release          Build optimized release output"
	@echo "  clean            Remove output for the selected configuration"
	@echo "  info             Show the active build configuration"
	@echo "  help             Show available build targets"
	@echo
	@echo "Variables: CXX, CONFIG=debug^|release, BUILD_ROOT, PROJECT_NAME,"
	@echo "           CPPFLAGS, CXXFLAGS, LDFLAGS, LDLIBS"

-include $(DEPS)