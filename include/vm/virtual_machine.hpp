#ifndef XVM_VIRTUAL_MACHINE_HPP
#define XVM_VIRTUAL_MACHINE_HPP

#include "devices/cpu.hpp"
#include "devices/framebuffer.hpp"
#include "devices/input_device.hpp"
#include "devices/io_bus.hpp"
#include "devices/ram.hpp"
#include "devices/system_clock.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace xvm {
    class VirtualMachine {
        public:
            VirtualMachine();
            ~VirtualMachine();

            VirtualMachine(const VirtualMachine&) = delete;
            VirtualMachine& operator=(const VirtualMachine&) = delete;

            bool loadRom(const std::string& filepath);
            bool loadRom(const std::vector<uint8_t>& romData);
            bool loadRom(const uint8_t* data, std::size_t size);

            void reset();
            void step();
            void runFrame(int instructionsPerFrame = 500);

            bool isHalted() const;
            void setHalted(bool halted);
            void setTrace(bool enable);

            void setButtonState(uint8_t buttonFlag, bool pressed);

            Cpu& getCpu() { return m_cpu; }
            const Cpu& getCpu() const { return m_cpu; }

            Ram& getRam() { return m_ram; }
            const Ram& getRam() const { return m_ram; }

            Framebuffer& getFramebuffer() { return m_framebuffer; }
            const Framebuffer& getFramebuffer() const { return m_framebuffer; }

            InputDevice& getInputDevice() { return m_inputDevice; }
            const InputDevice& getInputDevice() const { return m_inputDevice; }

            IoBus& getIoBus() { return m_ioBus; }
            const IoBus& getIoBus() const { return m_ioBus; }

            SystemClock& getClock() { return m_clock; }
            const SystemClock& getClock() const { return m_clock; }

        private:
            Ram m_ram;
            InputDevice m_inputDevice;
            IoBus m_ioBus;
            Framebuffer m_framebuffer;
            Cpu m_cpu;
            SystemClock m_clock;
    };
}

#endif