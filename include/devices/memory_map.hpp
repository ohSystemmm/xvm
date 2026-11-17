#ifndef XVM_MEMORY_MAP_HPP
#define XVM_MEMORY_MAP_HPP

#include <cstdint>

namespace xvm::memory_map {
    constexpr uint16_t ROM_START = 0x0000;
    constexpr uint16_t ROM_END   = 0x0FFF;

    constexpr uint16_t RAM_START = 0x1000;
    constexpr uint16_t RAM_END   = 0x3FFE;

    constexpr uint16_t IO_INPUT  = 0x3FFF;  
}

#endif