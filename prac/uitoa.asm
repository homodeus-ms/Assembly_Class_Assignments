
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
    xor dx, dx
    dec sp
    div si
    or dl, '0'
    dec di
    mov [di], dl
    cmp ax, 0
    jne get_char

    lea dx, [di]
    mov ah, 09h
    int 21h

    mov sp, bp

    pop di
    pop si
    pop bp

    ret

uitoa ENDP



