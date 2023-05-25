out0=$2300
numA0=$4000
numB0=$4004

    ldx #$FF
    txs

    sec
    lda numA0
    sbc numB0
    sta out0
    lda numA0+1
    sbc numB0+1
    sta out0+1
    lda numA0+2
    sbc numB0+2
    sta out0+2
    lda numA0+3
    sbc numB0+3
    sta out0+3
