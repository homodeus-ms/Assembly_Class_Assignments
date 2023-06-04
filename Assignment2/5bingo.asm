COL=5       ; using column count and just count
CENTER=12
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
; search order : cross> row > col
;=====================================

    .SUBROUTINE

;==== center value check ========

    sec
    ldx #CENTER
    lda table,x
    bpl .row            ; 센터값이 플러스면 크로스 체크 건너뜀

;==== search cross direction ====   

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
    bpl .row
    dey
    beq .bingo
    txa
    sbc #COL
    tax
    inx

    jmp .cr2loop

;================================

;===== search row direction ====

.row:            ; c=1
                                  
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
    beq .notyet

    lda offset
    sbc #1
    sta offset
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












