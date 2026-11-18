#ifndef XVM_IO_BUS_HPP
#define XVM_IO_BUS_HPP

#include <cstdint>
#include "ram.hpp"
#include "input_device.hpp"
#include "memory_map.hpp"

namespace xvm {
    class IoBus {
        public:
            IoBus(Ram& ram, InputDevice& inputDevice);
            ~IoBus();

            std::uint8_t read8(std::uint16_t address) const;
            void write8(std::uint16_t address, std::uint8_t value);

            void setRomWriteProtected(bool protectedRom);
            bool isRomWriteProtected() const;

        private:
            Ram& m_ram;
            InputDevice& m_inputDevice;
            bool m_romWriteProtected = false;
    };
}

#endif