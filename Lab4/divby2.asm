in0=$4000
in1=in0+1
in2=in1+1
in3=in2+1

out0=$2300
out1=out0+1
out2=out1+1
out3=out2+1

    ldx #$FF
    txs

    lda in3    ; 4
    tax        ; 2
    rol        ; 2
    txa        ; 2
    ror        ; 2
    sta out3   ; 4   16

    lda in2
    ror
    sta out2

    lda in1
    ror
    sta out1

    lda in0
    ror
    sta out0

    