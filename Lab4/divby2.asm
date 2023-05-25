in0=$4000
in1=in0+1
in2=in1+1
in3=in2+1

out0=$2300
out1=out0+1
out2=out1+1
out3=out2+1

keep_MSB=$00

    ldx #$FF
    txs

    clc
    lda in3
    tax
    and #$80
    sta keep_MSB
    txa
    ror
    ora keep_MSB
    sta out3

    lda in2
    ror
    sta out2

    lda in1
    ror
    sta out1

    lda in0
    ror
    sta out0

    