#include "vm/virtual_machine.hpp"
#include "devices/memory_map.hpp"

#include <fstream>
#include <spdlog/spdlog.h>

namespace xvm {
    VirtualMachine::VirtualMachine()
        : m_ram(),
          m_inputDevice(),
          m_framebuffer(Framebuffer::kDefaultWidth, Framebuffer::kDefaultHeight),
          m_ioBus(m_ram, m_inputDevice, &m_framebuffer),
          m_cpu(m_ioBus),
          m_clock() {
        m_clock.start();
    }

    VirtualMachine::~VirtualMachine() = default;

    bool VirtualMachine::loadRom(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            spdlog::error("VirtualMachine::loadRom: Failed to open file '{}'", filepath);
            return false;
        }

        const auto rawSize = file.tellg();
        if (rawSize <= 0) {
            spdlog::error("VirtualMachine::loadRom: File '{}' is empty", filepath);
            return false;
        }

        const auto fileSize = static_cast<std::size_t>(rawSize);
        constexpr std::size_t maxRomSize = memory_map::ROM_END - memory_map::ROM_START + 1;
        if (fileSize > maxRomSize) {
            spdlog::error("VirtualMachine::loadRom: File '{}' size ({} bytes) exceeds ROM size ({} bytes)", filepath, fileSize, maxRomSize);
            return false;
        }

        std::vector<uint8_t> buffer(fileSize);
        file.seekg(0, std::ios::beg);
        if (!file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(fileSize))) {
            spdlog::error("VirtualMachine::loadRom: Failed to read file '{}'", filepath);
            return false;
        }

        return loadRom(buffer);
    }

    bool VirtualMachine::loadRom(const uint8_t* data, std::size_t size) {
        if (!data || size == 0) {
            spdlog::error("VirtualMachine::loadRom: Null or empty ROM data provided");
            return false;
        }

        constexpr std::size_t maxRomSize = memory_map::ROM_END - memory_map::ROM_START + 1;
        if (size > maxRomSize) {
            spdlog::error("VirtualMachine::loadRom: ROM size ({} bytes) exceeds maximum capacity ({} bytes)", size, maxRomSize);
            return false;
        }

        std::vector<uint8_t> buffer(data, data + size);
        return loadRom(buffer);
    }

    bool VirtualMachine::loadRom(const std::vector<uint8_t>& romData) {
        constexpr std::size_t maxRomSize = memory_map::ROM_END - memory_map::ROM_START + 1;
        if (romData.empty() || romData.size() > maxRomSize) {
            spdlog::error("VirtualMachine::loadRom: Invalid ROM size ({} bytes)", romData.size());
            return false;
        }

        reset();

        m_ram.write(memory_map::ROM_START, romData); // write ROM to start of RAM

        m_ioBus.setRomWriteProtected(true); // disable runtime writes

        m_cpu.setProgramCounter(memory_map::ROM_START); // reset CPU to start of ROM
        m_cpu.setHalted(false);

        spdlog::info("VirtualMachine: Successfully loaded ROM ({} bytes)", romData.size());
        return true;
    }

    void VirtualMachine::reset() {
        m_ioBus.setRomWriteProtected(false);
        m_ram.reset();
        m_framebuffer.reset();
        m_inputDevice.reset();
        m_cpu.reset();
        spdlog::info("VirtualMachine: Hardware reset complete");
    }

    void VirtualMachine::step() {
        m_cpu.step();
    }

    void VirtualMachine::runFrame(int instructionsPerFrame) {
        for (int i = 0; i < instructionsPerFrame && !m_cpu.isHalted(); ++i) {
            m_cpu.step();
        }
    }

    bool VirtualMachine::isHalted() const {
        return m_cpu.isHalted();
    }

    void VirtualMachine::setHalted(bool halted) {
        m_cpu.setHalted(halted);
    }

    void VirtualMachine::setTrace(bool enable) {
        m_cpu.setTrace(enable);
    }

    void VirtualMachine::setButtonState(uint8_t buttonFlag, bool pressed) {
        m_inputDevice.setButtonState(buttonFlag, pressed);
    }
}