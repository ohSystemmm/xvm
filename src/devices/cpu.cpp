#include "devices/cpu.hpp"
#include "devices/framebuffer.hpp"
#include "devices/memory_map.hpp"

#include <algorithm>
#include <iterator>
#include <spdlog/spdlog.h>

namespace xvm {
    namespace {
        uint32_t paletteToRgba(uint8_t index) {
            static constexpr uint32_t kPalette[16] = {
                0xFF000000, // 0: Black
                0xFFFFFFFF, // 1: White
                0xFFFF3333, // 2: Red
                0xFF33FF33, // 3: Green
                0xFF3366FF, // 4: Blue
                0xFFFFFF33, // 5: Yellow
                0xFF33FFFF, // 6: Cyan
                0xFFFF33FF, // 7: Magenta
                0xFFFF8800, // 8: Orange
                0xFF444444, // 9: Dark Gray
                0xFFAAAAAA, // 10: Light Gray
                0xFF008800, // 11: Dark Green
                0xFF000088, // 12: Dark Blue
                0xFF880088, // 13: Purple
                0xFF884400, // 14: Brown
                0xFFFF88AA  // 15: Pink
            };
            if (index < 16) {
                return kPalette[index];
            }
            spdlog::warn("paletteToRgba: Invalid palette index {}", index);
            return 0xFF000000; // Default Black
        }
    }

    Cpu::Cpu(IoBus& ioBus, Framebuffer* framebuffer)
        : m_ioBus(ioBus),
          m_framebuffer(framebuffer),
          m_programCounter(memory_map::ROM_START),
          m_zeroflags(0),
          m_halted(false),
          m_trace(false) {
        std::fill(std::begin(m_registers), std::end(m_registers), 0);
    }

    Cpu::~Cpu() = default;

    void Cpu::reset() {
        std::fill(std::begin(m_registers), std::end(m_registers), 0);
        m_programCounter = memory_map::ROM_START;
        m_zeroflags = 0;
        m_halted = false;
    }

    void Cpu::step() {
        if (m_halted) {
            return;
        }

        if (m_programCounter + 3 >= Ram::kSize) {
            spdlog::error("Cpu::step: Program Counter 0x{:04X} out of bounds", m_programCounter);
            m_halted = true;
            return;
        }

        const uint8_t opcodeByte = m_ioBus.read8(m_programCounter);
        const uint8_t arg1       = m_ioBus.read8(m_programCounter + 1);
        const uint8_t arg2       = m_ioBus.read8(m_programCounter + 2);
        const uint8_t arg3       = m_ioBus.read8(m_programCounter + 3);

        const uint16_t currentPc = m_programCounter;
        m_programCounter += 4; // Advance ProgramCounter before execution

        if (m_trace) {
            spdlog::debug("CPU [PC:0x{:04X}] Op:0x{:02X} Args:[0x{:02X},0x{:02X},0x{:02X}] | R0-R7: [{:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X}] ZF:{}",
                          currentPc, opcodeByte, arg1, arg2, arg3,
                          m_registers[0], m_registers[1], m_registers[2], m_registers[3],
                          m_registers[4], m_registers[5], m_registers[6], m_registers[7],
                          m_zeroflags);
        }

        switch (static_cast<Opcode>(opcodeByte)) {
            case Opcode::NOP: {
                break;
            }

            case Opcode::LOAD_IMM: {
                if (arg1 < 8) {
                    m_registers[arg1] = arg2;
                }
                break;
            }

            case Opcode::LOAD_MEM: {
                const uint16_t addr = (static_cast<uint16_t>(arg2) << 8) | arg3;
                if (arg1 < 8) {
                    m_registers[arg1] = m_ioBus.read8(addr);
                }
                break;
            }

            case Opcode::STORE_MEM: {
                const uint16_t addr = (static_cast<uint16_t>(arg2) << 8) | arg3;
                if (arg1 < 8) {
                    m_ioBus.write8(addr, m_registers[arg1]);
                }
                break;
            }

            case Opcode::COPY_REG: {
                if (arg1 < 8 && arg2 < 8) {
                    m_registers[arg1] = m_registers[arg2];
                }
                break;
            }

            case Opcode::ADD: {
                if (arg1 < 8 && arg2 < 8) {
                    const uint16_t res = static_cast<uint16_t>(m_registers[arg1]) + m_registers[arg2];
                    m_registers[arg1] = static_cast<uint8_t>(res & 0xFF);
                    m_zeroflags = (m_registers[arg1] == 0) ? 1 : 0;
                }
                break;
            }

            case Opcode::SUB: {
                if (arg1 < 8 && arg2 < 8) {
                    const int16_t res = static_cast<int16_t>(m_registers[arg1]) - m_registers[arg2];
                    m_registers[arg1] = static_cast<uint8_t>(res & 0xFF);
                    m_zeroflags = (m_registers[arg1] == 0) ? 1 : 0;
                }
                break;
            }

            case Opcode::CMP: {
                if (arg1 < 8 && arg2 < 8) {
                    m_zeroflags = (m_registers[arg1] == m_registers[arg2]) ? 1 : 0;
                }
                break;
            }

            case Opcode::JUMP: {
                const uint16_t addr = (static_cast<uint16_t>(arg2) << 8) | arg3;
                m_programCounter = addr;
                break;
            }

            case Opcode::JUMP_ZERO: {
                const uint16_t addr = (static_cast<uint16_t>(arg2) << 8) | arg3;
                if (m_zeroflags != 0) {
                    m_programCounter = addr;
                }
                break;
            }

            case Opcode::JUMP_NOT_ZERO: {
                const uint16_t addr = (static_cast<uint16_t>(arg2) << 8) | arg3;
                if (m_zeroflags == 0) {
                    m_programCounter = addr;
                }
                break;
            }

            case Opcode::CLEAR_SCREEN: {
                if (m_framebuffer) {
                    const uint8_t colorIdx = (arg1 < 8) ? m_registers[arg1] : arg1;
                    m_framebuffer->clear(paletteToRgba(colorIdx));
                }
                break;
            }

            case Opcode::DRAW_PIXEL: {
                if (m_framebuffer && arg1 < 8 && arg2 < 8) {
                    const uint8_t colorIdx = (arg3 < 8) ? m_registers[arg3] : arg3;
                    m_framebuffer->setPixel(m_registers[arg1], m_registers[arg2], paletteToRgba(colorIdx));
                }
                break;
            }

            case Opcode::HALT: {
                m_halted = true;
                break;
            }

            case Opcode::AND: {
                if (arg1 < 8 && arg2 < 8) {
                    m_registers[arg1] &= m_registers[arg2];
                    m_zeroflags = (m_registers[arg1] == 0) ? 1 : 0;
                }
                break;
            }

            case Opcode::OR: {
                if (arg1 < 8 && arg2 < 8) {
                    m_registers[arg1] |= m_registers[arg2];
                    m_zeroflags = (m_registers[arg1] == 0) ? 1 : 0;
                }
                break;
            }

            default: {
                spdlog::warn("Cpu::step: Unknown opcode 0x{:02X} at PC 0x{:04X}", opcodeByte, currentPc);
                break;
            }
        }
    }
    
    void Cpu::setFramebuffer(Framebuffer* framebuffer) {
        m_framebuffer = framebuffer;
    }

    Framebuffer* Cpu::getFramebuffer() const {
        return m_framebuffer;
    }

    bool Cpu::isHalted() const {
        return m_halted;
    }

    void Cpu::setHalted(bool halted) {
        m_halted = halted;
    }

    void Cpu::setTrace(bool enable) {
        m_trace = enable;
    }

    bool Cpu::isTrace() const {
        return m_trace;
    }

    uint8_t Cpu::getRegister(std::size_t index) const {
        if (index < 8) {
            return m_registers[index];
        }
        return 0;
    }

    void Cpu::setRegister(std::size_t index, uint8_t value) {
        if (index < 8) {
            m_registers[index] = value;
        }
    }

    uint16_t Cpu::getProgramCounter() const {
        return m_programCounter;
    }

    void Cpu::setProgramCounter(uint16_t pc) {
        m_programCounter = pc;
    }

    bool Cpu::getZeroFlag() const {
        return m_zeroflags != 0;
    }

    void Cpu::setZeroFlag(bool zf) {
        m_zeroflags = zf ? 1 : 0;
    }
}