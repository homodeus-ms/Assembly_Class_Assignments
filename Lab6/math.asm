; ======= Rules =======
; data store $00~$0F
; preserve A,X,Y


min: ; (num0,num1 >> ret min)
;==================================
; return min between num0, num1
; num0, num1 passed through stack
; stack order : num1 > num0
; return value at 0x20
;==================================

num0=$00
num1=$01
retaddrl=$02
retaddrh=$03
POPCNT=retaddrh+1-num0
keepa=$04
keepx=$05
keepy=$06
retval=$20

    .SUBROUTINE

.keepval:
    sta keepa    ; use only A resister

.poploop:
    pla
    sta retaddrl ; Low
    pla
    sta retaddrh ; High
    pla 
    sta num0
    pla 
    sta num1

.checkmin:

    cmp num1
    bcs .storen1

.storen0:
    sta retval
    
    jmp .return

.storen1:
    lda num1
    sta retval

.return:
    lda num1
    pha
    lda num0
    pha
    lda retaddrh
    pha
    lda retaddrl
    pha

    lda keepa     ; recover A resister

    rts


max: ; (num0, num1 -> ret max | <X>)
;=====================================
; return max between num0 and num1
; num0=$10 num1=$11
; return value at X
;=====================================

    .SUBROUTINE

.keepval:
    sta keepa    ; use only A resister
    
.checkmax:
    lda num0
    cmp num1
    bcs .storen0

.storen1:
    ldx num1
    lda keepa

    rts
.storen0:
    ldx num0
    lda keepa

    rts

minmax: ;(num0,num1 > ret min, max at stack | <p>)
;=================================================
; return min, max between num0 & num1
; stack input order : retmax, retmin and L -> R  
; return order : min -> max
; clean up stack by callee
;=================================================

    .SUBROUTINE

.keepval:             ; minmax start : 0x8097
    sta keepa         ; curr stack : 01F9
    stx keepx         ; 01FD=04 01FC=02 01FB=80 01FA=2D

.poploop:
    pla
    sta retaddrl ; Low
    pla
    sta retaddrh ; High
    pla 
    sta num1
    pla 
    sta num0

    ; stack pos change
    tsx
    inx
    inx
    txs

.compare:   ; A = num0

    cmp num1
    bcs .num1min

.num0min:   ; A = num0 , should min val first when pla
    
    lda num1
    pha
    lda num0
    pha

    jmp .return

.num1min:   ; A = num0

    pha
    lda num1
    pha

.return:
    lda retaddrh
    pha
    lda retaddrl
    pha

    ldx keepx
    lda keepa

    rts

mmref: ;(num0, num1, out_min, out_max >> ret min, max) | <p>)
;============================================================
; return min and max between num0 & num1 at out_min, out_max
; num0 at $10
; stack order : right -> left
; stack clean up by this
;============================================================

    .SUBROUTINE

outmax=$08
outmin=$0A

.keepval:
    sta keepa
    sty keepy
    ldy #0

.poploop:
    pla
    sta retaddrl
    pla
    sta retaddrh
    pla
    sta num1
    pla
    sta outmin
    pla
    sta outmin+1
    pla
    sta outmax
    pla
    sta outmax+1

.compare:
    lda num0+$10
    cmp num1
    bcs .num1min

.num0min:    ; A = num0
    sta (outmin),y
    lda num1
    sta (outmax),y

    jmp .return

.num1min:
    sta (outmax),y
    lda num1
    sta (outmin),y

.return:
    lda retaddrh
    pha
    lda retaddrl
    pha

    lda keepa
    ldy keepy

    rts









    