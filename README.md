# XVM

XVM is a small virtual machine written in C++17. It includes a byte-oriented
CPU, a simple assembler, a memory-mapped input device, and a port-controlled
framebuffer. The included ROM demos draw on the 64 x 64 framebuffer and use
button input.

The CPU has eight 8-bit general-purpose registers and a 16-bit program
counter/address format. Its address space is 16 KiB. See the architecture
documents for the complete instruction and memory definitions:

* [Instruction Set Architecture](docs/ISA.md)
* [Memory and Device Map](docs/MMU.md)

## Requirements

The Makefile build uses:

* CMake
* GNU Make and a C++17 compiler (MinGW and g++)
* vcpkg with the `x64-mingw-dynamic` triplet

Set `VCPKG_ROOT` to your vcpkg installation directory if it is not located in
one of the paths automatically checked by the Makefile.

## Build and run

From the repository root, build the VM and assembler, assemble the default
ROM, then run it:



```sh
# Generate ROM
make rom/asm/rom.asm ROM_OUT=rom/bin/rom.bin

# Run xvm using default ROM
make run rom/bin/rom.bin
```

```sh
# Generate ROM
make rom/asm/animated_rom.asm ROM_OUT=rom/bin/animated_rom.bin

# Run xvm using default ROM
make run rom/bin/animated_rom.bin
```

## Assembler

The assembler accepts an assembly source file and an optional output path:

```sh
build/debug/xvm-asm.exe rom/asm/rom.asm -o rom/bin/rom.bin
```

If no input file is provided, it defaults to `rom/asm/rom.asm`; if `-o` is
omitted, output defaults to `rom/bin/rom.bin`. Instructions are fixed-width
four-byte encodings. For available operations, operands, and device ports,
refer to the [ISA](docs/ISA.md) and [memory map](docs/MMU.md).