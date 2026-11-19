; =================================
; XVM ROM
; 64x64 virtual framebuffer
; =================================

start:
    LOAD_IMM R0, 0
    CLEAR_SCREEN R0

    LOAD_IMM R2, 2

    ; -----------------------------
    ; Draw 'X'
    ; -----------------------------
    LOAD_IMM R0, 23
    LOAD_IMM R1, 28
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 23
    LOAD_IMM R1, 29
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 27
    LOAD_IMM R1, 28
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 27
    LOAD_IMM R1, 29
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 24
    LOAD_IMM R1, 30
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 25
    LOAD_IMM R1, 31
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 26
    LOAD_IMM R1, 32
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 26
    LOAD_IMM R1, 30
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 24
    LOAD_IMM R1, 32
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 23
    LOAD_IMM R1, 33
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 23
    LOAD_IMM R1, 34
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 27
    LOAD_IMM R1, 33
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 27
    LOAD_IMM R1, 34
    DRAW_PIXEL R0, R1, R2

    ; -----------------------------
    ; Draw 'V'
    ; -----------------------------
    LOAD_IMM R0, 29
    LOAD_IMM R1, 28
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 29
    LOAD_IMM R1, 29
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 29
    LOAD_IMM R1, 30
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 29
    LOAD_IMM R1, 31
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 29
    LOAD_IMM R1, 32
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 33
    LOAD_IMM R1, 28
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 33
    LOAD_IMM R1, 29
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 33
    LOAD_IMM R1, 30
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 33
    LOAD_IMM R1, 31
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 33
    LOAD_IMM R1, 32
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 30
    LOAD_IMM R1, 33
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 31
    LOAD_IMM R1, 34
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 32
    LOAD_IMM R1, 33
    DRAW_PIXEL R0, R1, R2

    ; -----------------------------
    ; Draw 'M'
    ; -----------------------------
    LOAD_IMM R0, 35
    LOAD_IMM R1, 28
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 35
    LOAD_IMM R1, 29
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 35
    LOAD_IMM R1, 30
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 35
    LOAD_IMM R1, 31
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 35
    LOAD_IMM R1, 32
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 35
    LOAD_IMM R1, 33
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 35
    LOAD_IMM R1, 34
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 39
    LOAD_IMM R1, 28
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 39
    LOAD_IMM R1, 29
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 39
    LOAD_IMM R1, 30
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 39
    LOAD_IMM R1, 31
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 39
    LOAD_IMM R1, 32
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 39
    LOAD_IMM R1, 33
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 39
    LOAD_IMM R1, 34
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 36
    LOAD_IMM R1, 29
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 37
    LOAD_IMM R1, 30
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 38
    LOAD_IMM R1, 29
    DRAW_PIXEL R0, R1, R2

    LOAD_IMM R0, 37
    LOAD_IMM R1, 31
    DRAW_PIXEL R0, R1, R2

    HALT