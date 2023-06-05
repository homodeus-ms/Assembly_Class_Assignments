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
    bne .bingo

.cross2:
    lda board+4
    bpl .row
    lda board+8
    bpl .row
    lda board+16
    bpl .row
    lda board+20
    bne .bingo


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












