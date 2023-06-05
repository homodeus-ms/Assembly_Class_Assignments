COLCNT=5       ; using as column count and just count
CVALUE=12
LENG=25
offidx=$10  ; using at won func
temp=$1F
num=$00
board=$80
callcnt=$1A

    lda #0
    sta callcnt

callnum: ; (num -> search and check) | <A,X,P>            
;==============================================
; search num in board, and change it
;==============================================

    .SUBROUTINE

    ldx #LENG-1                               ; 0x8047 A2
    lda num          ; can delete it(?)
    
.sloop:
    cmp board,x                              ; 0x804B D5
    beq .found                               ; opc=F0
    dex                                      ; opc=CA
    bpl .sloop                               ; opc=10

.fail:
    rts                                      ; opc=60

.found:

    ora #$80
    sta board,x
    inc callcnt
    
    rts

;=====================================



won: ; (no argument -> ret 01 or 00)
;=====================================
; check bingo, T(1) or F(0) in $01
; search order : cross> row > col
;=====================================

    .SUBROUTINE

;==== total called count check ==

    lda callcnt
    cmp #COLCNT
    bcc .notyet

;==== center value check ========

    sec
    ldx #CVALUE
    lda board,x
    bpl .row                      ; skip to '.cross' when centervalue is still plus

;==== search cross direction ====   

.cross:
    ldx #LENG-1

.cross1:
    lda board,x
    bpl .cross2
    txa 
    beq .bingo
    sbc #COLCNT+1
    tax
    
    jmp .cross1

.cross2:
    ldx #LENG-5

.cr2loop:
    lda board,x
    bpl .row
    txa
    sbc #COLCNT-1
    beq .bingo
    tax
    
    jmp .cr2loop

;================================

;===== search row direction ====

.row:                ; c=1
                                  
    ldx #LENG-1
    ldy #COLCNT 

.rowloop:
    lda board,x                 ; 0x8062 B5
    bpl .rowskip                ; 0x8064 10
    dex 
    dey
    beq .bingo
    
    jmp .rowloop

.rowskip:
    tya
    sta temp
    txa
    sbc temp
    bmi .col
    tax
    ldy #COLCNT
    
    jmp .rowloop

;===============================

;==== search col direction =====   ; c=0

.col: 
    sec

    ldx #LENG-1
    stx offidx        

    ldy #COLCNT

.colloop:
    lda board,x
    bpl .colskip
    txa
    sbc #COLCNT
    bmi .bingo
    tax 
    
    jmp .colloop

.colskip:
    dey
    beq .notyet

    lda offidx
    sbc #1
    sta offidx
    tax
    
    jmp .colloop

;================================

.notyet:
    lda #0
    sta $01
    rts

.bingo:
    lda #1
    sta $01
    rts












