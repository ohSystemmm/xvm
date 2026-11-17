#ifndef XVM_CPU_HPP
#define XVM_CPU_HPP

#include "devices/io_bus.hpp"
#include <cstdint>

namespace xvm {
    class Cpu {
    public:
        explicit Cpu(IoBus& ioBus);
        ~Cpu();

        void reset();
        void step();

    private:
        IoBus& m_ioBus;

        uint8_t m_registers[8];     // Accumulator
        uint16_t m_programCounter;  // 16-bit program counter
        uint8_t m_zeroflags;        // Zero flag
    };
}

#endif