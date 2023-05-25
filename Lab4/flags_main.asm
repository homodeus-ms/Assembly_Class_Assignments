    .PROCESSOR 6502
    .ORG $8000

    .INCLUDE "utils.asm"

    ;===DEFINE INPUT HERE===
    lda #$7D
    adc #$06
    ;========================

    .INCLUDE "flags.asm"

    ;===DEFINE ASSERTIONS HERE===
    assert $2300, $01
    assert $2301, $01
    assert $2302, $00
    assert $2303, $00
    ;============================

    termin

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000