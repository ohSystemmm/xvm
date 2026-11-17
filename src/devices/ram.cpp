#include "devices/ram.hpp"

#include <spdlog/spdlog.h>
#include <stdexcept>

namespace xvm {
    Ram::Ram() = default;
    Ram::~Ram() = default;

    std::vector<std::uint8_t> Ram::read(std::uint16_t address, std::size_t size) const {
        const auto start = static_cast<std::size_t>(address);
        if (start > m_memory.size() || size > m_memory.size() - start) {
            spdlog::error("RAM read exceeds 16 KiB address space: address=0x{:04X}, size={}", address, size);
            throw std::out_of_range("RAM read exceeds 16 KiB address space");
        }

        std::vector<std::uint8_t> data(size);
        for (std::size_t i = 0; i < size; ++i) {
            data[i] = m_memory[start + i];
        }
        return data;    
    }

    void Ram::write(std::uint16_t address, const std::vector<std::uint8_t>& data) {
        const auto start = static_cast<std::size_t>(address);
        if (start > m_memory.size() || data.size() > m_memory.size() - start) {
            spdlog::error("RAM write exceeds 16 KiB address space: address=0x{:04X}, size={}", address, data.size());
            throw std::out_of_range("RAM write exceeds 16 KiB address space");
        }

        for (std::size_t i = 0; i < data.size(); ++i) {
            m_memory[start + i] = data[i];
        }
    }

    uint8_t Ram::read8(std::uint16_t address) const {
        const auto addr = static_cast<std::size_t>(address);
        if (addr >= m_memory.size()) {
            spdlog::error("RAM read8 exceeds 16 KiB address space: address=0x{:04X}", address);
            throw std::out_of_range("RAM read8 exceeds 16 KiB address space");
        }
        return m_memory[addr];
    }

    void Ram::write8(std::uint16_t address, uint8_t value) {
        const auto addr = static_cast<std::size_t>(address);
        if (addr >= m_memory.size()) {
            spdlog::error("RAM write8 exceeds 16 KiB address space: address=0x{:04X}", address);
            throw std::out_of_range("RAM write8 exceeds 16 KiB address space");
        }
        m_memory[addr] = value;
    }

    void Ram::reset() {
        m_memory.fill(0);
    }
}