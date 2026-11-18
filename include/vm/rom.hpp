#ifndef XVM_DEMO_ROM_HPP
#define XVM_DEMO_ROM_HPP

#include "assembler.hpp"

#include <vector>
#include <cstdint>

namespace xvm {
    const int DISPLAY_WIDTH = 64; 
    const int DISPLAY_HEIGHT = 64; 

    inline void drawX(Assembler& asmblr, int offsetX, int offsetY) {
        for (int y = 0; y <= 1; y++) {
            asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 0);
            asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + y);
            asmblr.drawPixel(0, 1, 2);
        }

        for (int y = 0; y <= 1; y++) {
            asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 4);
            asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + y);
            asmblr.drawPixel(0, 1, 2);
        }

        for (int i = 1; i <= 3; i++) {
            asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + i);
            asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + i + 1);
            asmblr.drawPixel(0, 1, 2);
        }

        asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 3);
        asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + 2);
        asmblr.drawPixel(0, 1, 2);

        asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 1);
        asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + 4);
        asmblr.drawPixel(0, 1, 2);

        for (int y = 5; y <= 6; y++) {
            asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 0);
            asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + y);
            asmblr.drawPixel(0, 1, 2);
        }

        for (int y = 5; y <= 6; y++) {
            asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 4);
            asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + y);
            asmblr.drawPixel(0, 1, 2);
        }
    }

    inline void drawV(Assembler& asmblr, int offsetX, int offsetY) {
        for (int y = 0; y <= 4; y++) {
            asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 0);
            asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + y);
            asmblr.drawPixel(0, 1, 2);
        }

        for (int y = 0; y <= 4; y++) {
            asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 4);
            asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + y);
            asmblr.drawPixel(0, 1, 2);
        }

        asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 1);
        asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + 5);
        asmblr.drawPixel(0, 1, 2);

        asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 2);
        asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + 6);
        asmblr.drawPixel(0, 1, 2);

        asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 3);
        asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + 5);
        asmblr.drawPixel(0, 1, 2);
    }

    inline void drawM(Assembler& asmblr, int offsetX, int offsetY) {
        for (int y = 0; y <= 6; y++) {
            asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 0);
            asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + y);
            asmblr.drawPixel(0, 1, 2);
        }

        for (int y = 0; y <= 6; y++) {
            asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 4);
            asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + y);
            asmblr.drawPixel(0, 1, 2);
        }

        asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 1);
        asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + 1);
        asmblr.drawPixel(0, 1, 2);

        asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 2);
        asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + 2);
        asmblr.drawPixel(0, 1, 2);

        asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 3);
        asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + 1);
        asmblr.drawPixel(0, 1, 2);

        asmblr.emit(Opcode::LOAD_IMM, 0, offsetX + 2);
        asmblr.emit(Opcode::LOAD_IMM, 1, offsetY + 3);
        asmblr.drawPixel(0, 1, 2);
    }
    
    inline std::vector<uint8_t> createRom() {
        Assembler asmblr;

        asmblr.label("start");
        asmblr.clearScreen(0);

        asmblr.emit(Opcode::LOAD_IMM, 2, 2);

        int totalWidth = (3 * 5) + (2 * 1); 
        int startX = (DISPLAY_WIDTH - totalWidth) / 2;

        int charHeight = 7;
        int startY = (DISPLAY_HEIGHT - charHeight) / 2;

        drawX(asmblr, startX, startY);         // X
        drawV(asmblr, startX + 6, startY);     // V (5 Pixel + 1)
        drawM(asmblr, startX + 12, startY);    // M (5 + 1 + 5 + 1)

        asmblr.emit(Opcode::HALT, 0, 0, 0);

        return asmblr.assemble();
    }
}

#endif