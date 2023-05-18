    .PROCESSOR 6502
    .ORG $8000

    clc
    lda #$0A
    adc #$06
    sta $2200

    .ORG $FFFC,0
    .WORD $8000
    .WORD $0000