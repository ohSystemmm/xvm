#include "devices/ram.hpp"

#include <stdexcept>
#include <spdlog/spdlog.h>

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
}