PROJECT_NAME = xvm
CONFIG = debug
BUILD_ROOT = build-mingw
BUILD_DIR = $(BUILD_ROOT)/$(CONFIG)
CMAKE = cmake
CXX = g++
CMAKE_GENERATOR = MinGW Makefiles
CMAKE_MAKE_PROGRAM = $(MAKE)

VCPKG_TARGET_TRIPLET = x64-mingw-dynamic
VCPKG_HOST_TRIPLET = $(VCPKG_TARGET_TRIPLET)

ifeq ($(strip $(VCPKG_ROOT)),)
  ifneq ($(wildcard ../vcpkg/vcpkg.exe),)
    VCPKG_ROOT := $(abspath ../vcpkg)
  else ifneq ($(wildcard C:/Users/leonh/HolyMoly/003_Work/Work/Projects/vcpkg/vcpkg.exe),)
    VCPKG_ROOT := C:/Users/leonh/HolyMoly/003_Work/Work/Projects/vcpkg
  endif
endif

VCPKG_OVERLAY_TRIPLETS = $(VCPKG_ROOT)/triplets/community

ifeq ($(filter $(CONFIG),debug release),)
$(error CONFIG must be either 'debug' or 'release' (got '$(CONFIG)'))
endif

ifeq ($(strip $(VCPKG_ROOT)),)
$(error Set VCPKG_ROOT to your vcpkg installation directory or clone vcpkg to ../vcpkg)
endif

ifeq ($(strip $(VCPKG_OVERLAY_TRIPLETS)),)
$(error VCPKG_OVERLAY_TRIPLETS must point to the vcpkg community triplets directory)
endif

ifeq ($(CONFIG),debug)
CMAKE_CONFIG = Debug
else
CMAKE_CONFIG = Release
endif

.DEFAULT_GOAL := build
.PHONY: all configure build run debug release clean info help

all: build

configure:
	"$(CMAKE)" -S . -B "$(BUILD_DIR)" -G "$(CMAKE_GENERATOR)" \
		-DCMAKE_CXX_COMPILER="$(CXX)" \
		-DCMAKE_BUILD_TYPE=$(CMAKE_CONFIG) \
		-DCMAKE_MAKE_PROGRAM="$(CMAKE_MAKE_PROGRAM)" \
		-DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake" \
		-DVCPKG_TARGET_TRIPLET=$(VCPKG_TARGET_TRIPLET) \
		-DVCPKG_HOST_TRIPLET=$(VCPKG_HOST_TRIPLET) \
		-DVCPKG_OVERLAY_TRIPLETS="$(VCPKG_OVERLAY_TRIPLETS)"

build: configure
	"$(CMAKE)" --build "$(BUILD_DIR)" --config $(CMAKE_CONFIG)

run: build
	"$(BUILD_DIR)/$(PROJECT_NAME).exe"

debug:
	$(MAKE) CONFIG=debug build

release:
	$(MAKE) CONFIG=release build

clean:
	"$(CMAKE)" -E rm -rf "$(BUILD_ROOT)"

info:
	@echo Project:        $(PROJECT_NAME)
	@echo Compiler:       $(CXX)
	@echo Config:         $(CONFIG)
	@echo Build dir:      $(BUILD_DIR)
	@echo Generator:      $(CMAKE_GENERATOR)
	@echo Make program:   $(CMAKE_MAKE_PROGRAM)
	@echo Target Triplet: $(VCPKG_TARGET_TRIPLET)
	@echo Host Triplet:   $(VCPKG_HOST_TRIPLET)
	@echo vcpkg root:     $(VCPKG_ROOT)

help:
	@echo "xvm build targets:"
	@echo "  build            Configure with vcpkg and build (default: debug)"
	@echo "  run              Build and run the selected configuration"
	@echo "  debug            Build with debug settings"
	@echo "  release          Build optimized release output"
	@echo "  clean            Remove generated build directories"
	@echo "  info             Show the active build configuration"
	@echo "  help             Show available build targets"
	@echo
	@echo "Variables: CXX, CONFIG=debug^|release, BUILD_ROOT, CMAKE,"
	@echo "           VCPKG_ROOT, VCPKG_TARGET_TRIPLET, VCPKG_HOST_TRIPLET, VCPKG_OVERLAY_TRIPLETS"