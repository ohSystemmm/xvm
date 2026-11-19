#include "tools/assembler.hpp"

#include <spdlog/spdlog.h>
#include <string>

int main(int argc, char* argv[]) {
    std::string inputFile = "rom/asm/rom.asm";
    std::string outputFile = "rom/bin/rom.bin";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            spdlog::info("Usage: xvm-asm [input.asm] [-o output.bin]");
            spdlog::info("Default input:  example/rom.asm");
            spdlog::info("Default output: rom/demo.bin");
            return 0;
        } else if (inputFile == "example/rom.asm") {
            inputFile = arg;
        } else {
            outputFile = arg;
        }
    }

    spdlog::info("Assembling '{}' -> '{}'...", inputFile, outputFile);

    xvm::Assembler as;
    std::string err;

    if (!as.assembleFile(inputFile, &err)) {
        spdlog::error("Assembly failed: {}", err);
        return 1;
    }

    if (!as.writeToFile(outputFile)) {
        spdlog::error("Failed to write ROM binary to: {}", outputFile);
        return 1;
    }

    const auto bytes = as.assemble();
    spdlog::info("ROM assembled successfully: {} ({} bytes, {} instructions)", outputFile, bytes.size(), bytes.size() / 4);

    return 0;
}