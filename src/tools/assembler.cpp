#include "tools/assembler.hpp"

#include <spdlog/spdlog.h>
#include <string>

int main(int argc, char* argv[]) {
    std::string defaultInputFile = "rom/asm/rom.asm";
    std::string defaultOutputFile = "rom/bin/rom.bin";
    bool inputSpecified = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o") {
            if (i + 1 >= argc) {
                spdlog::error("Missing output path after -o");
                return 1;
            }
            defaultOutputFile = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            spdlog::info("Usage: xvm-asm [input.asm] [-o output.bin]");
            spdlog::info("Default input:  {}", defaultInputFile);
            spdlog::info("Default output: {}", defaultOutputFile);
            return 0;
        } else if (!inputSpecified) {
            defaultInputFile = arg;
            inputSpecified = true;
        } else {
            spdlog::error("Unexpected argument: {}", arg);
            return 1;
        }
    }

    spdlog::info("Assembling '{}' -> '{}'...", defaultInputFile, defaultOutputFile);

    xvm::Assembler as;
    std::string err;

    if (!as.assembleFile(defaultInputFile, &err)) {
        spdlog::error("Assembly failed: {}", err);
        return 1;
    }

    if (!as.writeToFile(defaultOutputFile)) {
        spdlog::error("Failed to write ROM binary to: {}", defaultOutputFile);
        return 1;
    }

    const auto bytes = as.assemble();
    spdlog::info("ROM assembled successfully: {} ({} bytes, {} instructions)", defaultOutputFile, bytes.size(), bytes.size() / 4);

    return 0;
}