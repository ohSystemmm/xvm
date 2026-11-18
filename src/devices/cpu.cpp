#include "devices/cpu.hpp"
#include "devices/memory_map.hpp"
#include "vm/opcode.hpp"

#include <algorithm>
#include <iterator>
#include <spdlog/spdlog.h>

namespace xvm {

    Cpu::Cpu(IoBus& ioBus)
        : m_ioBus(ioBus),
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

            case Opcode::PORT_OUT: {
                if (arg2 < 8) {
                    m_ioBus.writePort(arg1, m_registers[arg2]);
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