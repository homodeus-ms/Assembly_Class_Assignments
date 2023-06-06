COLCNT=5       ; using as column count and just count
CVALUE=12
LENG=25
offidx=$10  ; using at won func
temp=$1F
stackp=$1E

num=$00
board=$80
calledn=$11


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
    
    rts

;=====================================



won: ; (no argument -> ret 01 or 00)
;=====================================
; check bingo, T(1) or F(0) in $01
; search order : cross> row > col
;=====================================

    .SUBROUTINE

;==== center value check ========

    sec
    lda board+CVALUE
    bpl .row                     ; skip to '.cross' when centervalue is still plus

;======== check cross  ==========   

.cross:
    lda board
    bpl .cross2
    lda board+6
    bpl .cross2
    lda board+18
    bpl .cross2
    lda board+24
    bmi .bingo

.cross2:
    lda board+4
    bpl .row
    lda board+8
    bpl .row
    lda board+16
    bpl .row
    lda board+20
    bmi .bingo


;================================

;===== search row direction ====

.row:

    ldx #LENG-1
    ldy #COLCNT

.rowloop:

    lda board,x
    bmi .rcheck

.changex:

    sty temp
    txa
    sbc temp
    bmi .col1
    tax
    ldy #COLCNT

    jmp .rowloop

.rcheck:
   
    dex
    dey
    bne .rowloop
    jmp .bingo


;===============================

;==== search col direction =====   ; c=0

.col1:
    
    ldx #COLCNT
    ldy #0

.ccheck:

    dex
    bmi .col2
    lda board,x
    bmi .pushx
    
    jmp .ccheck

.pushx:
    
    txa
    pha
    iny
    jmp .ccheck
    

.col2:

    tya
    beq .notyet
    
    pla
    tax
    
    lda board+5,x
    bpl .nextrow
  
    lda board+10,x
    bpl .nextrow

    lda board+15,x
    bpl .nextrow

    lda board+20,x
    bmi .bingo

.nextrow:

    dey
    jmp .col2


;================================

.notyet:
    lda #0
    sta $01
    rts

.bingo:            ;(0x80CB maybe?)
    lda #1
    sta $01
    rts












