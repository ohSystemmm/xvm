#include "devices/memory_map.hpp"

namespace xvm::memory_map {
    bool isRom(uint16_t address) {
        return address <= ROM_END;
    }

    bool isRam(uint16_t address) {
        return address >= RAM_START && address <= RAM_END;
    }

    bool isIo(uint16_t address) {
        return address == IO_INPUT;
    }

    bool isValidAddress(uint16_t address) {
        return address < TOTAL_SIZE;
    }
}