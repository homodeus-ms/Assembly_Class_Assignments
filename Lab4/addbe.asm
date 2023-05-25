out0=$2300
numA0=$4003
numB0=$4007

    ldx #$FF
    txs

    clc
    lda numA0
    adc numB0
    sta out0
    lda numA0-1
    adc numB0-1
    sta out0+1
    lda numA0-2
    adc numB0-2
    sta out0+2
    lda numA0-3
    adc numB0-3
    sta out0+3
