; =================================
; XVM ROM – Movable Pixel
; 64x64 virtual framebuffer
; =================================
;
; 0 (0x00000000) Black
; 1 (0xFFFFFFFF) White 
; 2 (0xFFFF3333) Red
; 3 (0xFF33FF33) Green
; 4 (0xFF3366FF) Blue
; 5 (0xFFFFFF33) Yellow
; 6 (0xFF33FFFF) Cyan
; 7 (0xFFFF33FF) Magenta
; 8 (0xFFFF8800) Orange
; 9 (0xFF444444) Dark Grey
; 10 (0xFFAAAAAA) Light Grey
; 11 (0xFF008800) Dark Green
; 12 (0xFF000088) Dark Blue
; 13 (0xFF880088) Purple
; 14 (0xFF884400) Brown
; 15 (0xFFFF88AA) Pink

start:
    LOAD_IMM R5, 32
    LOAD_IMM R6, 32
    LOAD_IMM R0, 1
    PORT_OUT FB_CLEAR, R0
    JUMP draw_pixel

main_loop:
    LOAD_MEM R3, 0x3FFF
    LOAD_IMM R7, 0
    CMP R3, R7
    JUMP_ZERO main_loop

    COPY_REG R4, R3
    LOAD_IMM R7, 1
    AND R4, R7
    LOAD_IMM R7, 0
    CMP R4, R7
    JUMP_ZERO skip_up
    LOAD_IMM R7, 0
    CMP R6, R7
    JUMP_ZERO skip_up
    LOAD_IMM R7, 1
    SUB R6, R7

skip_up:
    COPY_REG R4, R3
    LOAD_IMM R7, 2
    AND R4, R7
    LOAD_IMM R7, 0
    CMP R4, R7
    JUMP_ZERO skip_down
    LOAD_IMM R7, 63
    CMP R6, R7
    JUMP_ZERO skip_down
    LOAD_IMM R7, 1
    ADD R6, R7

skip_down:
    COPY_REG R4, R3
    LOAD_IMM R7, 4
    AND R4, R7
    LOAD_IMM R7, 0
    CMP R4, R7
    JUMP_ZERO skip_left
    LOAD_IMM R7, 0
    CMP R5, R7
    JUMP_ZERO skip_left
    LOAD_IMM R7, 1
    SUB R5, R7

skip_left:
    COPY_REG R4, R3
    LOAD_IMM R7, 8
    AND R4, R7
    LOAD_IMM R7, 0
    CMP R4, R7
    JUMP_ZERO skip_right
    LOAD_IMM R7, 63
    CMP R5, R7
    JUMP_ZERO skip_right
    LOAD_IMM R7, 1
    ADD R5, R7

skip_right:
    LOAD_IMM R0, 1
    PORT_OUT FB_CLEAR, R0

draw_pixel:
    LOAD_IMM R2, 2
    PORT_OUT FB_X, R5
    PORT_OUT FB_Y, R6
    PORT_OUT FB_DRAW, R2
    JUMP main_loop
