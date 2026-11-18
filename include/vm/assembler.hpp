#ifndef XVM_ASSEMBLER_HPP
#define XVM_ASSEMBLER_HPP

#include "devices/cpu.hpp"
#include "devices/io_ports.hpp"
#include "vm/opcode.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace xvm {
    class Assembler {
        public:
            void label(const std::string& name) {
                m_labels[name] = static_cast<uint16_t>(m_instructions.size() * 4);
            }

            void emit(Opcode op, uint8_t a1 = 0, uint8_t a2 = 0, uint8_t a3 = 0) {
                m_instructions.push_back({op, a1, a2, a3, ""});
            }

            void jump(Opcode op, const std::string& targetLabel) {
                m_instructions.push_back({op, 0, 0, 0, targetLabel});
            }

            void portOut(uint8_t port, uint8_t reg) {
                emit(Opcode::PORT_OUT, port, reg, 0);
            }

            void drawPixel(uint8_t regX = 0, uint8_t regY = 1, uint8_t regColor = 2) {
                portOut(io_ports::FB_X, regX);
                portOut(io_ports::FB_Y, regY);
                portOut(io_ports::FB_DRAW, regColor);
            }

            void clearScreen(uint8_t regColor = 0) {
                portOut(io_ports::FB_CLEAR, regColor);
            } 

            std::vector<uint8_t> assemble() const {
                std::vector<uint8_t> bytes;
                bytes.reserve(m_instructions.size() * 4);
                for (const auto& instr : m_instructions) {
                    uint8_t hi = instr.a2;
                    uint8_t lo = instr.a3;
                    if (!instr.targetLabel.empty()) {
                        auto it = m_labels.find(instr.targetLabel);
                        if (it != m_labels.end()) {
                            hi = static_cast<uint8_t>((it->second >> 8) & 0xFF);
                            lo = static_cast<uint8_t>(it->second & 0xFF);
                        }
                    }
                    bytes.push_back(static_cast<uint8_t>(instr.op));
                    bytes.push_back(instr.a1);
                    bytes.push_back(hi);
                    bytes.push_back(lo);
                }
                return bytes;
            }

        private:
            struct Item {
                Opcode op;
                uint8_t a1;
                uint8_t a2;
                uint8_t a3;
                std::string targetLabel;
            };
            std::vector<Item> m_instructions;
            std::unordered_map<std::string, uint16_t> m_labels;
    };
}

#endif