#ifndef XVM_IO_PORTS_HPP
#define XVM_IO_PORTS_HPP

#include <cstdint>

namespace xvm::io_ports {
    // Framebuffer I/O ports
    constexpr uint8_t FB_X     = 0x01;
    constexpr uint8_t FB_Y     = 0x02;
    constexpr uint8_t FB_DRAW  = 0x03; // Draws pixel
    constexpr uint8_t FB_CLEAR = 0x04; // Clears the screen 

    constexpr uint8_t FB_COLOR = FB_DRAW;
}

#endif
