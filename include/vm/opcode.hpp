#ifndef XVM_OPCODE_HPP
#define XVM_OPCODE_HPP

#include <inttypes.h>

namespace xvm {
    enum class Opcode : uint8_t {
        NOP           = 0x00,
        LOAD_IMM      = 0x01,
        LOAD_MEM      = 0x02,
        STORE_MEM     = 0x03,
        COPY_REG      = 0x04,
        ADD           = 0x05,
        SUB           = 0x06,
        CMP           = 0x07,
        JUMP          = 0x08,
        JUMP_ZERO     = 0x09,
        JUMP_NOT_ZERO = 0x0A,
        PORT_OUT      = 0x0B,
        HALT          = 0x0D,
        AND           = 0x0E,
        OR            = 0x0F
    };
}

#endif