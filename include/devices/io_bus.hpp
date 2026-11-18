#ifndef XVM_IO_BUS_HPP
#define XVM_IO_BUS_HPP

#include "ram.hpp"
#include "input_device.hpp"

#include <cstdint>

namespace xvm {
    class Framebuffer;

    class IoBus {
        public:
            IoBus(Ram& ram, InputDevice& inputDevice, Framebuffer* framebuffer = nullptr);
            ~IoBus();

            std::uint8_t read8(std::uint16_t address) const;
            void write8(std::uint16_t address, std::uint8_t value);

            void writePort(std::uint8_t port, std::uint8_t value);
            std::uint8_t readPort(std::uint8_t port) const;

            void setFramebuffer(Framebuffer* framebuffer);
            Framebuffer* getFramebuffer() const;

            void setRomWriteProtected(bool protectedRom);
            bool isRomWriteProtected() const;

        private:
            Ram& m_ram;
            InputDevice& m_inputDevice;
            Framebuffer* m_framebuffer = nullptr;
            bool m_romWriteProtected = false;
    };
}

#endif