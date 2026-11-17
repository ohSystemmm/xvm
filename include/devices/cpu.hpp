#ifndef XVM_CPU_HPP
#define XVM_CPU_HPP

#include "devices/io_bus.hpp"
#include <cstddef>
#include <cstdint>

namespace xvm {
    class Framebuffer;

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
        CLEAR_SCREEN  = 0x0B, // Replace with generalized instruction (e.g., PORT_OUT to framebuffer)
        DRAW_PIXEL    = 0x0C, // Replace with generalized instruction (e.g., PORT_OUT to framebuffer)
        HALT          = 0x0D,
        AND           = 0x0E,
        OR            = 0x0F
    };

    class Cpu {
        public:
            explicit Cpu(IoBus& ioBus, Framebuffer* framebuffer = nullptr);
            ~Cpu();

            void reset();
            void step();

            void setFramebuffer(Framebuffer* framebuffer);
            Framebuffer* getFramebuffer() const;

            bool isHalted() const;
            void setHalted(bool halted);

            void setTrace(bool enable);
            bool isTrace() const;

            uint8_t getRegister(std::size_t index) const;
            void setRegister(std::size_t index, uint8_t value);

            uint16_t getProgramCounter() const;
            void setProgramCounter(uint16_t pc);

            bool getZeroFlag() const;
            void setZeroFlag(bool zf);

        private:
            IoBus& m_ioBus;
            Framebuffer* m_framebuffer = nullptr;

            uint8_t m_registers[8]{};     // General purpose registers R0 - R7
            uint16_t m_programCounter = 0;// 16-bit program counter
            uint8_t m_zeroflags = 0;      // Zero flag (0 or 1)
            bool m_halted = false;
            bool m_trace = false;
        };
}

#endif