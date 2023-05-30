buffer=$C000
out=$2300
accumVal=$11

    lda buffer
    asl
    asl
    sec
    sbc #4
    tax

first:

    lda buffer+2
    sta out

    lda buffer+3
    sta out+1
 
    lda buffer+4
    sta out+2

    lda buffer+5
    sta out+3

    txa
    beq check

accum:

    lda out+3
    eor buffer+5,x
    sta out+3
    
    lda out+2
    eor buffer+4,x
    sta out+2

    lda out+1
    eor buffer+3,x
    sta out+1

    lda out
    eor buffer+2,x
    sta out

    txa
    sbc #4
    tax
    
    bne accum
  
check:            ; 0x806c c가 1 상태임

    lda buffer+1
    bit out+3
    beq continue
    
    bit out+2
    beq continue

    bit out+1
    beq continue

    bit out
    bne end
  
keep:

    lda out
    sta accumVal
    
    lda out+1
    sta accumVal+1
    
    lda out+2
    sta accumVal+2
 
    lda out+3
    sta accumVal+3

continue:        ; 0x8083 여전히 c는 1
    
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

loop:   ; 0x8058

    lda out
    eor accumVal
    sta out

    lda out+1
    eor accumVal+1
    sta out+1

    lda out+2
    eor accumVal+2
    sta out+2

    lda out+3
    eor accumVal+3
    sta out+3

    jmp check

end:
