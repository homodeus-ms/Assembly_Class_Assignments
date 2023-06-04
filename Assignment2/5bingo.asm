COL=5       ; using column count and just count
offset=$10  ; using won func
num=$00

callnum: ; (num, table) | <A,X,P>            
;====================================
; search num in table, and change it
;====================================

    .SUBROUTINE

    ldx #LEN-1                               ; 0x8047 A2
    lda num   ; can delete it
    
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
; search order : row > col > cross
;=====================================

    .SUBROUTINE

;===== search row direction ====

.row: 
    sec                                 ; 0x805A 38
    lda #LEN-1
    sta offset

    tax
    ldy #COL

.rowloop:
    lda table,x                 ; 0x8062 B5
    bpl .rowskip                ; 0x8064 10
    dex 
    dey
    beq .bingo
    
    jmp .rowloop

.rowskip:
    lda offset
    sbc #COL
    bmi .col
    sta offset
    tax
    ldy #COL
    jmp .rowloop

;===============================

;==== search col direction =====   ; c=0

.col: 
    sec
    lda #LEN-1
    sta offset

    tax
    ldy #COL

.colloop:
    lda table,x
    bpl .colskip
    txa
    sbc #COL
    bmi .bingo
    tax 
    
    jmp .colloop

.colskip:
    dey
    beq .cross

    lda offset
    sbc #1
    sta offset
    tax
    
    jmp .colloop

;================================

;==== search cross direction ====   ; c=1

.cross:
    ldx #LEN-1
    ldy #COL

.cross1:
    lda table,x
    bpl .cross2
    txa 
    beq .bingo
    sbc #COL
    tax
    dex
    
    jmp .cross1

.cross2:
    ldx #LEN-5

.cr2loop:
    lda table,x
    bpl .notyet
    dey
    beq .bingo
    txa
    sbc #COL
    tax
    inx

    jmp .cr2loop

;================================

.notyet:
    lda #0
    sta $01
    rts

.bingo:
    lda #1
    sta $01
    rts












