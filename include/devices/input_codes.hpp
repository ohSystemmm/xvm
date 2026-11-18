#ifndef XVM_INPUT_CODES_HPP
#define XVM_INPUT_CODES_HPP

#include <inttypes.h>

namespace xvm::buttons {
    constexpr uint8_t UP    = 1 << 0; // 0x01
    constexpr uint8_t DOWN  = 1 << 1; // 0x02
    constexpr uint8_t LEFT  = 1 << 2; // 0x04
    constexpr uint8_t RIGHT = 1 << 3; // 0x08
    constexpr uint8_t ACTION = 1 << 4; // 0x10
}

#endif