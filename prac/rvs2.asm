TITLE rvs2

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA

msg DB "Hello World!$"

.CODE
.STARTUP

    lea bl, msg
    xor bh, bh
    push bx
    call rvs
    add sp,2

    mov ah, 4ch
    xor al, al
    int 21h

rvs PROC

    mov bp, sp
    mov bx, [bp+2]
    xor si, si

rvs_loop:
    mov ax, [bx+si]
    cmp ax, '$'
    je eloop

    inc si
    call rvs_loop

return:
    dec si
    lea dx, [bx+si]
    mov ah, 02h
    int 21h

eloop:
    ret

rvs ENDP

END
