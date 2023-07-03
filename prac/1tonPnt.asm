TITLE 1ton print

.DOSSEG
.8086
.NO87
.MODEL TINY

.CODE
.STARTUP
; ===== MACROS =====
print MACRO
    mov ah, 09h
    int 21h
ENDM

exitProgram MACRO
    mov ax, 4c00h
    int 21h
ENDM
; ===== END MACROS =====

    mov ax, 100
    push ax
    call sum
    add sp, 2

    push ax
    call uitoa
    add sp, 2

    exitProgram


sum PROC NEAR
    push bp
    mov bp, sp
    mov cx, [bp+4]
    xor ax, ax
sloop:
    add ax, cx
    loop sloop

    pop bp
    ret
sum ENDP

uitoa PROC NEAR

    push bp
    push si
    push di
    mov bp, sp

    mov ax, [bp+8]
    dec sp
    lea di, [bp-1]
    mov BYTE PTR [di], '$'
    mov si, 10

get_char:
    dec sp
    xor dx, dx
    div si
    or dl, '0'
    dec di
    mov [di], dl
    cmp ax, 0
    jne get_char

    lea dx, [di]

    print

    mov sp, bp
    pop di
    pop si
    pop bp

    ret

uitoa ENDP

END
