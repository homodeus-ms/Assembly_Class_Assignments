    .PROCESSOR 6502
    .ORG $8000

    .INCLUDE "utils.asm"

    ;===DEFINE INPUTS HERE===
    ; 0x000000BA (186)
    lda #$BA
    sta $4003
    lda #$00
    sta $4002
    sta $4001
    sta $4000

    ; 0xFFFFFB2F (-1233)
    lda #$2F
    sta $4007
    lda #$FB
    sta $4006
    lda #$FF
    sta $4005
    lda #$FF
    sta $4004
    ;========================

    .INCLUDE "subbe.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $8B
    assert $2301, $05
    assert $2302, $00
    assert $2303, $00
    ;============================

    termin

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000