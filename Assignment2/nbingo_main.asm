    .PROCESSOR 6502
    .ORG $8000
DIM = 5
table = $C000
    .INCLUDE "utils.asm"
    
    ldx #$FF
    txs
    lda #DIM
    pha
    lda #$C0
    pha
    lda #$00
    pha
    jsr won
    assert $0001, $01
    termin
    .INCLUDE "nbingo.asm"
    .ORG $C000, 0
    .BYTE $67, $8F, $6F, $6D, $FD
    .BYTE $3D, $AC, $46, $B8, $EE
    .BYTE $32, $D7, $EA, $10, $85
    .BYTE $96, $C3, $CC, $38, $E6
    .BYTE $97, $33, $06, $1C, $32
    .ORG $FFFC, 0
    .WORD $8000
    .WORD $0000