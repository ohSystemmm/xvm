#ifndef XVM_PALETTE_HPP
#define XVM_PALETTE_HPP

#include <inttypes.h>
#include <spdlog/spdlog.h>

namespace xvm {
    inline uint32_t paletteToRgba(uint8_t index) {
        static constexpr uint32_t kPalette[16] = {
            0xFF000000, // 0: Black
            0xFFFFFFFF, // 1: White
            0xFFFF3333, // 2: Red
            0xFF33FF33, // 3: Green
            0xFF3366FF, // 4: Blue
            0xFFFFFF33, // 5: Yellow
            0xFF33FFFF, // 6: Cyan
            0xFFFF33FF, // 7: Magenta
            0xFFFF8800, // 8: Orange
            0xFF444444, // 9: Dark Gray
            0xFFAAAAAA, // 10: Light Gray
            0xFF008800, // 11: Dark Green
            0xFF000088, // 12: Dark Blue
            0xFF880088, // 13: Purple
            0xFF884400, // 14: Brown
            0xFFFF88AA  // 15: Pink
        };
        if (index < 16) {
            return kPalette[index];
        }
        spdlog::warn("paletteToRgba: Invalid palette index {}", index);
        return 0xFF000000; // Default Black
    }
}

#endif