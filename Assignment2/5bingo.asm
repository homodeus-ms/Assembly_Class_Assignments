;COLCNT=5       ; using as column count and just count
;CVALUE=12
;offsidx=$10  ; using at won func
;num4comp=$00

callnum: ; (num4comp, table) | <A,X,P>            
;====================================
; search num in table, and change it
;====================================

    ;.SUBROUTINE

    ldx #LEN-1                               ; 0x8047 A2
    lda $00   ; can delete it
    
.searchloop:
    cmp table,x                              ; 0x804B D5
    beq .found                               ; F0
    dex                                      ; CA
    bpl .searchloop                          ; 10

.notfound:
    rts                  ; 60

.found:
    lda table,x                              ; 0x8053 B5
    ora #$80
    sta table,x
    rts

;=====================================



won:
;=====================================
; check bingo, T(1) or F(0) in $01
; search order : cross> row > col
;=====================================

    ;.SUBROUTINE

;==== center value check ========

    sec
    ldx #12
    lda table,x
    bpl .row            ; skip '.cross' when centervalue is still plus

;==== search cross direction ====   

.cross:
    ldx #LEN-1
    ldy #5

.cross1:
    lda table,x
    bpl .cross2
    txa 
    beq .bingo
    sbc #5
    tax
    dex
    
    jmp .cross1

.cross2:
    ldx #LEN-5

.cr2loop:
    lda table,x
    bpl .row
    dey
    beq .bingo
    txa
    sbc #5
    tax
    inx

    jmp .cr2loop

;================================

;===== search row direction ====

.row:            ; c=1
                                  
    lda #LEN-1
    sta $10

    tax
    ldy #5

.rowloop:
    lda table,x                 ; 0x8062 B5
    bpl .rowskip                ; 0x8064 10
    dex 
    dey
    beq .bingo
    
    jmp .rowloop

.rowskip:
    lda $10
    sbc #5
    bmi .col
    sta $10
    tax
    ldy #5
    jmp .rowloop

;===============================

;==== search col direction =====   ; c=0

.col: 
    sec
    lda #LEN-1
    sta $10

    tax
    ldy #5

.colloop:
    lda table,x
    bpl .colskip
    txa
    sbc #5
    bmi .bingo
    tax 
    
    jmp .colloop

.colskip:
    dey
    beq .notyet

    lda $10
    sbc #1
    sta $10
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












