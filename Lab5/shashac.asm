buffer=$C000
out=$2300
count=$10
    ldx #$FF
    txs

    lda buffer
    asl
    asl
    clc
    sta count

round:
    ldx #0

first:
    lda buffer+2  ; 바로빠져나가는 경우:2+32+9 43사이클후 체크
    sta out       ; 두번도는경우 : 2+20+9 (39) + 51+9(60) = 99
  
    lda buffer+3
    sta out+1
 
    lda buffer+4
    sta out+2

    lda buffer+5
    sta out+3

    ldx #4
    cpx count
    bcs check

accum:      ; 0x802B

    lda out
    eor buffer+2,x
    sta out

    lda out+1
    eor buffer+3,x
    sta out+1

    lda out+2
    eor buffer+4,x
    sta out+2

    lda out+3
    eor buffer+5,x
    sta out+3

    txa       ; 0x804F
    adc #4
    cmp count
    tax
    bcc accum

check:            ; 0x8057 c가 1 상태임

    lda out
    and buffer+1
    beq continue
    
    lda out+1
    and buffer+1
    beq continue

    lda out+2
    and buffer+1
    beq continue

    lda out+3
    and buffer+1
    bne end

continue:        ; 여전히 c는 1
    
    lda out
    asl
    ora #1
    sta out

    lda out+1
    rol
    ora #1
    sta out+1

    lda out+2
    rol
    ora #1
    sta out+2

    lda out+3
    rol
    ora #1
    sta out+3

    ldx #0
    jmp accum

end:
