#ifndef XVM_ASSEMBLER_HPP
#define XVM_ASSEMBLER_HPP

#include "devices/cpu.hpp"

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

    // Demonstrative built-in demo ROM for XVM.
    inline std::vector<uint8_t> createDefaultDemoRom() {
        Assembler as;

        // Variables in RAM:
        // 0x1000: Player X (32)
        // 0x1001: Player Y (32)
        // 0x1002: Target X (16)
        // 0x1003: Target Y (16)
        // 0x1004: Player Color (6 = Cyan)
        // 0x1005: Target Color (5 = Yellow)
        // 0x1006: Step (1)
        // 0x1007: Screen Mask (63)

        as.emit(Opcode::LOAD_IMM, 0, 32);
        as.emit(Opcode::STORE_MEM, 0, 0x10, 0x00); // Player X = 32
        as.emit(Opcode::STORE_MEM, 0, 0x10, 0x01); // Player Y = 32

        as.emit(Opcode::LOAD_IMM, 0, 16);
        as.emit(Opcode::STORE_MEM, 0, 0x10, 0x02); // Target X = 16
        as.emit(Opcode::STORE_MEM, 0, 0x10, 0x03); // Target Y = 16

        as.emit(Opcode::LOAD_IMM, 0, 6);           // Cyan
        as.emit(Opcode::STORE_MEM, 0, 0x10, 0x04); // Player Color = 6

        as.emit(Opcode::LOAD_IMM, 0, 5);           // Yellow
        as.emit(Opcode::STORE_MEM, 0, 0x10, 0x05); // Target Color = 5

        as.emit(Opcode::LOAD_IMM, 0, 1);
        as.emit(Opcode::STORE_MEM, 0, 0x10, 0x06); // Step = 1

        as.emit(Opcode::LOAD_IMM, 0, 63);
        as.emit(Opcode::STORE_MEM, 0, 0x10, 0x07); // Mask = 63

        as.label("main_loop");
        as.emit(Opcode::CLEAR_SCREEN, 0);

        as.emit(Opcode::LOAD_MEM, 7, 0x3F, 0xFF);        // Read Input byte from 0x3FFF into R7

        // Check UP (0x01)
        as.emit(Opcode::LOAD_IMM, 6, 0x01);
        as.emit(Opcode::COPY_REG, 5, 7);
        as.emit(Opcode::AND, 5, 6);
        as.jump(Opcode::JUMP_ZERO, "check_down");
        as.emit(Opcode::LOAD_MEM, 1, 0x10, 0x01); // Player Y
        as.emit(Opcode::LOAD_MEM, 6, 0x10, 0x06); // Step
        as.emit(Opcode::SUB, 1, 6);
        as.emit(Opcode::LOAD_MEM, 6, 0x10, 0x07); // Mask 63
        as.emit(Opcode::AND, 1, 6);
        as.emit(Opcode::STORE_MEM, 1, 0x10, 0x01);

        as.label("check_down");
        // Check DOWN (0x02)
        as.emit(Opcode::LOAD_IMM, 6, 0x02);
        as.emit(Opcode::COPY_REG, 5, 7);
        as.emit(Opcode::AND, 5, 6);
        as.jump(Opcode::JUMP_ZERO, "check_left");
        as.emit(Opcode::LOAD_MEM, 1, 0x10, 0x01); // Player Y
        as.emit(Opcode::LOAD_MEM, 6, 0x10, 0x06); // Step
        as.emit(Opcode::ADD, 1, 6);
        as.emit(Opcode::LOAD_MEM, 6, 0x10, 0x07); // Mask 63
        as.emit(Opcode::AND, 1, 6);
        as.emit(Opcode::STORE_MEM, 1, 0x10, 0x01);

        as.label("check_left");
        // Check LEFT (0x04)
        as.emit(Opcode::LOAD_IMM, 6, 0x04);
        as.emit(Opcode::COPY_REG, 5, 7);
        as.emit(Opcode::AND, 5, 6);
        as.jump(Opcode::JUMP_ZERO, "check_right");
        as.emit(Opcode::LOAD_MEM, 0, 0x10, 0x00); // Player X
        as.emit(Opcode::LOAD_MEM, 6, 0x10, 0x06); // Step
        as.emit(Opcode::SUB, 0, 6);
        as.emit(Opcode::LOAD_MEM, 6, 0x10, 0x07); // Mask 63
        as.emit(Opcode::AND, 0, 6);
        as.emit(Opcode::STORE_MEM, 0, 0x10, 0x00);

        as.label("check_right");
        // Check RIGHT (0x08)
        as.emit(Opcode::LOAD_IMM, 6, 0x08);
        as.emit(Opcode::COPY_REG, 5, 7);
        as.emit(Opcode::AND, 5, 6);
        as.jump(Opcode::JUMP_ZERO, "check_collision");
        as.emit(Opcode::LOAD_MEM, 0, 0x10, 0x00); // Player X
        as.emit(Opcode::LOAD_MEM, 6, 0x10, 0x06); // Step
        as.emit(Opcode::ADD, 0, 6);
        as.emit(Opcode::LOAD_MEM, 6, 0x10, 0x07); // Mask 63
        as.emit(Opcode::AND, 0, 6);
        as.emit(Opcode::STORE_MEM, 0, 0x10, 0x00);

        as.label("check_collision");
        // Collision check: Player X == Target X && Player Y == Target Y
        as.emit(Opcode::LOAD_MEM, 0, 0x10, 0x00); // Player X
        as.emit(Opcode::LOAD_MEM, 2, 0x10, 0x02); // Target X
        as.emit(Opcode::CMP, 0, 2);
        as.jump(Opcode::JUMP_NOT_ZERO, "draw_scene");

        as.emit(Opcode::LOAD_MEM, 1, 0x10, 0x01); // Player Y
        as.emit(Opcode::LOAD_MEM, 3, 0x10, 0x03); // Target Y
        as.emit(Opcode::CMP, 1, 3);
        as.jump(Opcode::JUMP_NOT_ZERO, "draw_scene");

        // Relocate target: Target X = (Target X + 19) & 63, Target Y = (Target Y + 29) & 63
        as.emit(Opcode::LOAD_IMM, 6, 19);
        as.emit(Opcode::ADD, 2, 6);
        as.emit(Opcode::LOAD_MEM, 6, 0x10, 0x07);
        as.emit(Opcode::AND, 2, 6);
        as.emit(Opcode::STORE_MEM, 2, 0x10, 0x02);

        as.emit(Opcode::LOAD_IMM, 6, 29);
        as.emit(Opcode::ADD, 3, 6);
        as.emit(Opcode::LOAD_MEM, 6, 0x10, 0x07);
        as.emit(Opcode::AND, 3, 6);
        as.emit(Opcode::STORE_MEM, 3, 0x10, 0x03);

        as.label("draw_scene");
        as.emit(Opcode::LOAD_MEM, 2, 0x10, 0x02); // Target X
        as.emit(Opcode::LOAD_MEM, 3, 0x10, 0x03); // Target Y
        as.emit(Opcode::LOAD_MEM, 4, 0x10, 0x05); // Target Color
        as.emit(Opcode::DRAW_PIXEL, 2, 3, 4);

        // Draw Player dot
        as.emit(Opcode::LOAD_MEM, 0, 0x10, 0x00); // Player X
        as.emit(Opcode::LOAD_MEM, 1, 0x10, 0x01); // Player Y
        as.emit(Opcode::LOAD_MEM, 4, 0x10, 0x04); // Player Color
        as.emit(Opcode::DRAW_PIXEL, 0, 1, 4);

        as.jump(Opcode::JUMP, "main_loop"); // Loop

        return as.assemble();
    }
}

#endif