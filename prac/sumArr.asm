TITLE Get sum of array

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA
nums DB 2,4,6,8,10

.CODE
.STARTUP

    ARR_LENGTH EQU 5

    lea ax, nums
    push ax
    call sum
    add sp, 2

    push ax
    call uitoa
    add sp, 2

    mov ax, 4c00h
    int 21h

sum PROC NEAR
    push bp
    mov bp, sp
    mov bx, [bp+4]
    mov si, ARR_LENGTH
    
    xor ax, ax

s_loop:
    dec si
    mov cl, BYTE PTR [bx+si]
    add ax, cx
    cmp si, 0
    jne s_loop

    pop bp
    ret
    

sum ENDP

INCLUDE uitoa.asm

END