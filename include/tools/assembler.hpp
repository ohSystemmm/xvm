#ifndef XVM_ASSEMBLER_HPP
#define XVM_ASSEMBLER_HPP

#include "devices/io_ports.hpp"
#include "vm/opcode.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

namespace xvm {
    class Assembler {
        public:
            void clear() {
                m_instructions.clear();
                m_labels.clear();
            }

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

            bool writeToFile(const std::string& filepath) const {
                auto bytes = assemble();
                std::ofstream file(filepath, std::ios::binary);
                if (!file.is_open()) {
                    spdlog::error("Failed to open output file for writing: {}", filepath);
                    return false;
                }
                file.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
                if (!file.good()) {
                    spdlog::error("Failed to write assembled bytes to file: {}", filepath);
                    return false;
                }
                return true;
            }

            bool assembleFile(const std::string& filepath, std::string* errorMsg = nullptr) {
                std::ifstream file(filepath);
                if (!file.is_open()) {
                    if (errorMsg != nullptr) {
                        *errorMsg = "Could not open file: " + filepath;
                    }
                    spdlog::error("Could not open file: {}", filepath);
                    return false;
                }
                std::string line;
                std::size_t lineNum = 0;
                while (std::getline(file, line)) {
                    ++lineNum;
                    if (!parseLine(line, lineNum, errorMsg)) {
                        return false;
                    }
                }
                return true;
            }

            bool assembleString(const std::string& source, std::string* errorMsg = nullptr) {
                std::istringstream stream(source);
                std::string line;
                std::size_t lineNum = 0;
                while (std::getline(stream, line)) {
                    ++lineNum;
                    if (!parseLine(line, lineNum, errorMsg)) {
                        return false;
                    }
                }
                return true;
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

            static std::string trim(const std::string& str) {
                const auto start = str.find_first_not_of(" \t\r\n");
                if (start == std::string::npos) {
                    return "";
                }
                const auto end = str.find_last_not_of(" \t\r\n");
                return str.substr(start, end - start + 1);
            }

            static std::string toUpper(std::string str) {
                std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
                    return static_cast<char>(std::toupper(c));
                });
                return str;
            }

            static bool parseRegister(const std::string& token, uint8_t& outReg) {
                std::string t = toUpper(trim(token));
                if (t.empty()) {
                    return false;
                }
                if (t.front() == 'R') {
                    t = t.substr(1);
                }
                try {
                    int val = std::stoi(t);
                    if (val >= 0 && val < 8) {
                        outReg = static_cast<uint8_t>(val);
                        return true;
                    }
                } catch (...) {}
                return false;
            }

            static bool parseValue(const std::string& token, uint16_t& outVal) {
                std::string t = toUpper(trim(token));
                if (t.empty()) {
                    return false;
                }

                if (t == "FB_X") { 
                    outVal = io_ports::FB_X; 
                    return true; 
                }
                
                if (t == "FB_Y") { 
                    outVal = io_ports::FB_Y; 
                    return true; 
                }
                
                if (t == "FB_DRAW" || t == "FB_COLOR") { 
                    outVal = io_ports::FB_DRAW; 
                    return true; 
                }
                
                if (t == "FB_CLEAR") { 
                    outVal = io_ports::FB_CLEAR; 
                    return true; 
                }

                try {
                    int base = 10;
                    if (t.rfind("0X", 0) == 0) {
                        base = 16;
                        t = t.substr(2);
                    }
                    unsigned long val = std::stoul(t, nullptr, base);
                    outVal = static_cast<uint16_t>(val);
                    return true;
                } catch (...) {}
                return false;
            }

            bool parseLine(const std::string& rawLine, std::size_t lineNum, std::string* errorMsg) {
                std::string line = rawLine;

                const auto commentPos = line.find_first_of(";#");
                if (commentPos != std::string::npos) {
                    line = line.substr(0, commentPos);
                }

                const auto dslash = line.find("//");
                if (dslash != std::string::npos) {
                    line = line.substr(0, dslash);
                }

                line = trim(line);
                if (line.empty()) {
                    return true;
                }

                const auto colonPos = line.find(':');

                if (colonPos != std::string::npos) {
                    std::string lbl = trim(line.substr(0, colonPos));
                    label(lbl);

                    line = trim(line.substr(colonPos + 1));
                    if (line.empty()) {
                        return true;
                    }
                }

                std::vector<std::string> tokens;
                std::string current;

                for (char c : line) {
                    if (c == ',' || std::isspace(static_cast<unsigned char>(c))) {
                        if (!current.empty()) {
                            tokens.push_back(current);
                            current.clear();
                        }
                    } else {
                        current.push_back(c);
                    }
                }

                if (!current.empty()) {
                    tokens.push_back(current);
                }

                if (tokens.empty()) {
                    return true;
                }

                const std::string mnemonic = toUpper(tokens[0]);

                auto requireArgs = [&](std::size_t count) -> bool {
                    if (tokens.size() - 1 < count) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": " + mnemonic +
                                        " expects " + std::to_string(count) + " arguments";
                        }
                        spdlog::error("Line {}: {} expects {} arguments", lineNum, mnemonic, count);
                        return false;
                    }
                    return true;
                };

                if (mnemonic == "NOP") {
                    emit(Opcode::NOP);
                } 
                
                else if (mnemonic == "LOAD_IMM") {
                    if (!requireArgs(2)) {
                        return false;
                    }

                    uint8_t reg;
                    uint16_t val;
                    if (!parseRegister(tokens[1], reg) || !parseValue(tokens[2], val)) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": invalid LOAD_IMM arguments";
                        }
                        spdlog::error("Line {}: invalid LOAD_IMM arguments", lineNum);
                        return false;
                    }

                    emit(Opcode::LOAD_IMM, reg, static_cast<uint8_t>(val & 0xFF));
                } 
                
                else if (mnemonic == "LOAD_MEM") {
                    if (!requireArgs(2)) {
                        return false;
                    }

                    uint8_t reg;
                    uint16_t addr;
                    if (!parseRegister(tokens[1], reg) || !parseValue(tokens[2], addr)) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": invalid LOAD_MEM arguments";
                        }
                        spdlog::error("Line {}: invalid LOAD_MEM arguments", lineNum);
                        return false;
                    }

                    emit(Opcode::LOAD_MEM, reg, static_cast<uint8_t>((addr >> 8) & 0xFF), static_cast<uint8_t>(addr & 0xFF));
                } 
                
                else if (mnemonic == "STORE_MEM") {
                    if (!requireArgs(2)) {
                        return false;
                    }

                    uint8_t reg;
                    uint16_t addr;
                    if (!parseRegister(tokens[1], reg) || !parseValue(tokens[2], addr)) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": invalid STORE_MEM arguments";
                        }
                        spdlog::error("Line {}: invalid STORE_MEM arguments", lineNum);
                        return false;
                    }

                    emit(Opcode::STORE_MEM, reg, static_cast<uint8_t>((addr >> 8) & 0xFF), static_cast<uint8_t>(addr & 0xFF));
                } 
                
                else if (mnemonic == "COPY_REG") {
                    if (!requireArgs(2)) {
                        return false;
                    }

                    uint8_t dst;
                    uint8_t src;
                    if (!parseRegister(tokens[1], dst) || !parseRegister(tokens[2], src)) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": invalid COPY_REG arguments";
                        }
                        spdlog::error("Line {}: invalid COPY_REG arguments", lineNum);
                        return false;
                    }

                    emit(Opcode::COPY_REG, dst, src);
                } 
                
                else if (mnemonic == "ADD") {
                    if (!requireArgs(2)) {
                        return false;
                    }

                    uint8_t rA;
                    uint8_t rB;
                    if (!parseRegister(tokens[1], rA) || !parseRegister(tokens[2], rB)) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": invalid ADD arguments";
                        }
                        spdlog::error("Line {}: invalid ADD arguments", lineNum);
                        return false;
                    }

                    emit(Opcode::ADD, rA, rB);
                } 
                
                else if (mnemonic == "SUB") {
                    if (!requireArgs(2)) {
                        return false;
                    }

                    uint8_t rA;
                    uint8_t rB;
                    if (!parseRegister(tokens[1], rA) || !parseRegister(tokens[2], rB)) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": invalid SUB arguments";
                        }
                        spdlog::error("Line {}: invalid SUB arguments", lineNum);
                        return false;
                    }

                    emit(Opcode::SUB, rA, rB);
                } 
                
                else if (mnemonic == "CMP") {
                    if (!requireArgs(2)) {
                        return false;
                    }

                    uint8_t rA;
                    uint8_t rB;
                    if (!parseRegister(tokens[1], rA) || !parseRegister(tokens[2], rB)) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": invalid CMP arguments";
                        }
                        spdlog::error("Line {}: invalid CMP arguments", lineNum);
                        return false;
                    }

                    emit(Opcode::CMP, rA, rB);
                } 
                
                else if (mnemonic == "JUMP" || mnemonic == "JMP") {
                    if (!requireArgs(1)) {
                        return false;
                    }

                    uint16_t addr;
                    if (parseValue(tokens[1], addr)) {
                        emit(Opcode::JUMP, 0, static_cast<uint8_t>((addr >> 8) & 0xFF), static_cast<uint8_t>(addr & 0xFF));
                    } else {
                        jump(Opcode::JUMP, tokens[1]);
                    }
                } 
                
                else if (mnemonic == "JUMP_ZERO" || mnemonic == "JZ") {
                    if (!requireArgs(1)) {
                        return false;
                    }
                    
                    uint16_t addr;
                    if (parseValue(tokens[1], addr)) {
                        emit(Opcode::JUMP_ZERO, 0, static_cast<uint8_t>((addr >> 8) & 0xFF), static_cast<uint8_t>(addr & 0xFF));
                    } else {
                        jump(Opcode::JUMP_ZERO, tokens[1]);
                    }
                } 
                
                else if (mnemonic == "JUMP_NOT_ZERO" || mnemonic == "JNZ") {
                    if (!requireArgs(1)) {
                        return false;
                    }

                    uint16_t addr;
                    if (parseValue(tokens[1], addr)) {
                        emit(Opcode::JUMP_NOT_ZERO, 0, static_cast<uint8_t>((addr >> 8) & 0xFF), static_cast<uint8_t>(addr & 0xFF));
                    } else {
                        jump(Opcode::JUMP_NOT_ZERO, tokens[1]);
                    }
                } 
                
                else if (mnemonic == "PORT_OUT") {
                    if (!requireArgs(2)) {
                        return false;
                    }

                    uint16_t port;
                    uint8_t reg;
                    if (!parseValue(tokens[1], port) || !parseRegister(tokens[2], reg)) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": invalid PORT_OUT arguments";
                        }
                        spdlog::error("Line {}: invalid PORT_OUT arguments", lineNum);
                        return false;
                    }

                    portOut(static_cast<uint8_t>(port & 0xFF), reg);
                } 
                
                else if (mnemonic == "HALT") {
                    emit(Opcode::HALT);
                } 
                
                else if (mnemonic == "AND") {
                    if (!requireArgs(2)) {
                        return false;
                    }

                    uint8_t rA;
                    uint8_t rB;
                    if (!parseRegister(tokens[1], rA) || !parseRegister(tokens[2], rB)) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": invalid AND arguments";
                        }
                        spdlog::error("Line {}: invalid AND arguments", lineNum);
                        return false;
                    }

                    emit(Opcode::AND, rA, rB);
                } 
                
                else if (mnemonic == "OR") {
                    if (!requireArgs(2)) {
                        return false;
                    }

                    uint8_t rA;
                    uint8_t rB;
                    if (!parseRegister(tokens[1], rA) || !parseRegister(tokens[2], rB)) {
                        if (errorMsg != nullptr) {
                            *errorMsg = "Line " + std::to_string(lineNum) + ": invalid OR arguments";
                        }
                        spdlog::error("Line {}: invalid OR arguments", lineNum);
                        return false;
                    }

                    emit(Opcode::OR, rA, rB);
                } 
                
                else if (mnemonic == "DRAW_PIXEL") {
                    uint8_t rx = 0;
                    uint8_t ry = 1;
                    uint8_t rc = 2;

                    if (tokens.size() > 1) {
                        if (!parseRegister(tokens[1], rx)) {
                            if (errorMsg != nullptr) {
                                *errorMsg = "Line " + std::to_string(lineNum) + ": invalid register X for DRAW_PIXEL";
                            }
                            spdlog::error("Line {}: invalid register X for DRAW_PIXEL", lineNum);
                            return false;
                        }
                    }

                    if (tokens.size() > 2) {
                        if (!parseRegister(tokens[2], ry)) {
                            if (errorMsg != nullptr) {
                                *errorMsg = "Line " + std::to_string(lineNum) + ": invalid register Y for DRAW_PIXEL";
                            }
                            spdlog::error("Line {}: invalid register Y for DRAW_PIXEL", lineNum);
                            return false;
                        }
                    }

                    if (tokens.size() > 3) {
                        if (!parseRegister(tokens[3], rc)) {
                            if (errorMsg != nullptr) {
                                *errorMsg = "Line " + std::to_string(lineNum) + ": invalid color register for DRAW_PIXEL";
                            }
                            spdlog::error("Line {}: invalid color register for DRAW_PIXEL", lineNum);
                            return false;
                        }
                    }

                    drawPixel(rx, ry, rc);
                } 
                
                else if (mnemonic == "CLEAR_SCREEN") {
                    uint8_t rc = 0;
                    if (tokens.size() > 1) {
                        if (!parseRegister(tokens[1], rc)) {
                            if (errorMsg != nullptr) {
                                *errorMsg = "Line " + std::to_string(lineNum) + ": invalid color register for CLEAR_SCREEN";
                            }
                            spdlog::error("Line {}: invalid color register for CLEAR_SCREEN", lineNum);
                            return false;
                        }
                    }
                    clearScreen(rc);
                } else {
                    if (errorMsg != nullptr) {
                        *errorMsg = "Line " + std::to_string(lineNum) + ": unknown instruction '" + tokens[0] + "'";
                    }
                    spdlog::error("Line {}: unknown instruction '{}'", lineNum, tokens[0]);
                    return false;
                }
                return true;
            }
    };
}

#endif
