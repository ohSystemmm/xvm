#include "devices/io_bus.hpp"

#include <spdlog/spdlog.h>

namespace xvm {
    IoBus::IoBus(Ram& ram, InputDevice& inputDevice)
        : m_ram(ram), m_inputDevice(inputDevice), m_romWriteProtected(false) {}

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

    void IoBus::setRomWriteProtected(bool protectedRom) {
        m_romWriteProtected = protectedRom;
    }

    bool IoBus::isRomWriteProtected() const {
        return m_romWriteProtected;
    }
}