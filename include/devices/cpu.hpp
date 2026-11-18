#ifndef XVM_CPU_HPP
#define XVM_CPU_HPP

#include "devices/io_bus.hpp"

#include <cstddef>
#include <cstdint>

namespace xvm {
    class Cpu {
        public:
            explicit Cpu(IoBus& ioBus);
            ~Cpu();

            void reset();
            void step();

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

            uint8_t m_registers[8]{};      // General purpose registers R0 - R7
            uint16_t m_programCounter = 0; // 16-bit program counter
            uint8_t m_zeroflags = 0;       // Zero flag (0 or 1)
            bool m_halted = false;
            bool m_trace = false;
    };
}

#endif