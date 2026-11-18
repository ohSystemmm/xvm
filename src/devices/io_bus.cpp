#include "devices/io_bus.hpp"
#include "devices/framebuffer.hpp"
#include "devices/io_ports.hpp"

#include <spdlog/spdlog.h>

namespace xvm {
    IoBus::IoBus(Ram& ram, InputDevice& inputDevice, Framebuffer* framebuffer)
        : m_ram(ram),
          m_inputDevice(inputDevice),
          m_framebuffer(framebuffer),
          m_romWriteProtected(false) {}

    IoBus::~IoBus() = default;

    std::uint8_t IoBus::read8(std::uint16_t address) const {
        if (!memory_map::isValidAddress(address)) {
            spdlog::error("IoBus::read8: Address 0x{:04X} out of bounds", address);
            return 0;
        }

        if (address == memory_map::IO_INPUT) {
            return m_inputDevice.readInput();
        }

        return m_ram.read8(address);
    }

    void IoBus::write8(std::uint16_t address, std::uint8_t value) {
        if (!memory_map::isValidAddress(address)) {
            spdlog::error("IoBus::write8: Address 0x{:04X} out of bounds", address);
            return;
        }

        if (address == memory_map::IO_INPUT) {
            // IO_INPUT is read-only (input register)
            return;
        }

        if (m_romWriteProtected && memory_map::isRom(address)) {
            spdlog::warn("IoBus::write8: Blocked write to protected ROM at 0x{:04X}", address);
            return;
        }

        m_ram.write8(address, value);
    }

    void IoBus::writePort(std::uint8_t port, std::uint8_t value) {
        switch (port) {
            case io_ports::FB_X:
            case io_ports::FB_Y:
            case io_ports::FB_DRAW:
            case io_ports::FB_CLEAR:
                if (m_framebuffer) {
                    m_framebuffer->writePort(port, value);
                } else {
                    spdlog::warn("IoBus::writePort: Framebuffer not attached for port 0x{:02X}", port);
                }
                break;
            default:
                spdlog::warn("IoBus::writePort: Unhandled port 0x{:02X}", port);
                break;
        }
    }

    std::uint8_t IoBus::readPort(std::uint8_t port) const {
        switch (port) {
            case io_ports::FB_X:
            case io_ports::FB_Y:
                if (m_framebuffer) {
                    return m_framebuffer->readPort(port);
                }
                break;
            default:
                spdlog::warn("IoBus::readPort: Unhandled port 0x{:02X}", port);
                break;
        }
        return 0;
    }

    void IoBus::setFramebuffer(Framebuffer* framebuffer) {
        m_framebuffer = framebuffer;
    }

    Framebuffer* IoBus::getFramebuffer() const {
        return m_framebuffer;
    }

    void IoBus::setRomWriteProtected(bool protectedRom) {
        m_romWriteProtected = protectedRom;
    }

    bool IoBus::isRomWriteProtected() const {
        return m_romWriteProtected;
    }
}