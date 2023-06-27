
callnum: ; (btbl, n, num | <A,X,Y,S,btbl>    
;=========================================
; search num in btbl, and change it
;
; btbl : bingo board address
; n : bingo board size
; num : called number (7bit)
;
; stack order : num >> n >> b High >> b Low
;               >> ret High >> ret Low
; stack cleanup by callee
;=========================================

retaddr=$10
btbl=$12
size=$14
num=$15
lastidx=$16
lastrow=$17
POPCNT=%11111010    ; -6


    .SUBROUTINE

    ldx #POPCNT

.poploop:
    pla
    sta num+1,x ; ret L(10) > ret H(11) > board L(12)
                ; > board H(13) > n(14) > num(15)
    inx
    bne .poploop

    ; get board's last index
    lda size
    tax
    dex
    stx lastrow    ; size-1, 테스트에 callnum없이 won을 호출하는 경우가
                             ; 있는 것 같음. won함수에서 다시 구하므로 안해도 됨 
    beq .strlidx   ;  in case of size=1
    clc

.getlidx:          ; get (size * size)
    adc size
    dex
    bne .getlidx

.strlidx:
    sbc #0         ; c=0 , it's the same with (size * size) - 1
    sta lastidx
    tay            ; y=2d last index
    
    lda num

.search:
    cmp (btbl),y
    beq .found    
    dey
    bpl .search

.fail:
    lda retaddr+1
    pha
    lda retaddr
    pha

    rts

.found:
    ora #$80
    sta (btbl),y
    lda retaddr+1
    pha
    lda retaddr
    pha

    rts

;============================================



won: ; (btbl,n -> ret T/F at $01 | <A,X,Y>
;=============================================
; find bingo in n * n size btbl
; n : board size, btblt : board address
; stack : right -> left
; btbl=$12, size=$14, lastidx=$16, lastrow=$17
; return True : 1 in $01 / False : 0 in $01
; stack clean up by caller
;=============================================

    .SUBROUTINE

temp=$1A
lastcol=$1B

.getdata:
    pla
    sta retaddr
    pla
    sta retaddr+1
    pla
    sta btbl
    pla
    sta btbl+1
    pla
    sta size
    clc
    tax
    dex
    stx lastrow           
    inx

.getlidx:
    
    dex
    beq .chksize
    adc size
    jmp .getlidx

.chksize:
    ; prepare some variables
    tax 
    dex
    stx lastidx          ; save lastidx 
    
    sbc lastrow          ; size=5 기준으로 A(25) - lastrow(4) 이므로 -1을 더 빼줘야 하는데  
                         ; 현재 c=0 상태이므로 굳이 sec를 안하고 그냥 빼버림 
    sta lastcol

;===== check cross direction =====

.cross1:
    sec
    ldy lastidx

.cloop1:

    lda (btbl),y
    bmi .ccheck1

.cross2:

    lda lastidx
    sbc lastrow
    tay

.cloop2:
    
    lda (btbl),y
    bpl .row
    
.ccheck2:
    iny
    tya
    sbc size
    beq .bingo
    tay
    jmp .cloop2

.ccheck1:
    dey
    bmi .bingo
    tya
    sbc size
    tay
    jmp .cloop1

;================================

;===== search row direction =====

.row:                       ; c=0

    ldx size
    ldy lastidx
    sec

.rowloop:

    lda (btbl),y
    bmi .rcheck

.changex:

    stx temp
    tya
    sbc temp
    bmi .col1
    tay
    ldx size

    jmp .rowloop

.rcheck:
   
    dey
    dex
    bne .rowloop
    jmp .bingo


;============================================

;=========== search col direction ===========   ; c=0
; 1. col1 : 한 줄 확인하면서 negtive value가 있는
;          곳만 stack에 push
; 2. col2 : 스택에 저장된 곳만 pull해서 
;           세로방향 확인
;============================================

.col1:
    
    ldy lastidx
    iny
    ldx #0

.ccheck:
    
    dey
    cpy lastcol
    bmi .col2
    lda (btbl),y
    bmi .pushx
    
    jmp .ccheck

.pushx:
    
    tya
    pha
    inx
    jmp .ccheck
    

.col2:
    
    txa
    beq .notyet

    sec
    pla
    sbc size
    tay

.colloop:   
 
    lda (btbl),y
    bpl .nextrow
    tya
    sbc size
    bmi .bingo
    tay
    jmp .colloop

.nextrow:

    dex
    jmp .col2


;============================================

.notyet:
    lda #0
    sta $01

    lda retaddr+1
    pha
    lda retaddr
    pha

    rts

.bingo:            
    lda #1
    sta $01

    lda retaddr+1
    pha
    lda retaddr
    pha

    rts
    
    
    
























