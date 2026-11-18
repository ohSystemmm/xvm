#ifndef XVM_RAM_HPP
#define XVM_RAM_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace xvm {
    class Ram {
        public:
            static constexpr std::size_t kSize = 16 * 1024; // 16 KiB

            Ram();
            ~Ram();

            std::vector<std::uint8_t> read(std::uint16_t address, std::size_t size) const;
            void write(std::uint16_t address, const std::vector<std::uint8_t>& data);

            uint8_t read8(std::uint16_t address) const;
            void write8(std::uint16_t address, uint8_t value);

            void reset();

            const uint8_t* data() const { return m_memory.data(); }
            std::size_t size() const { return m_memory.size(); }

        private:
            std::array<std::uint8_t, kSize> m_memory{};
    };
}

#endif