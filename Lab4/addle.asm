out0=$2300
out1=out0+1
out2=out1+1
out3=out2+1    

numA0=$4000
numB0=$4004

    ldx #$FF
    txs

    clc
    lda numA0
    adc numB0
    sta out0
    lda numA0+1
    adc numB0+1
    sta out1
    lda numA0+2
    adc numB0+2
    sta out2
    lda numA0+3
    adc numB0+3
    sta out3

