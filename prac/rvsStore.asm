TITLE rvsstr

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA
msg DB "12345$"
res DB 5 DUP (?), '$'
MSG_LENGTH EQU 5

.CODE
.STARTUP

    lea ax, res
    push ax
    lea ax, msg
    push ax
    call rvs
    add sp, 4

    lea dx, res
    mov ah, 09h
    int 21h

    mov ax, 4c00h
    int 21h

rvs PROC NEAR
    push bp
    mov bp, sp
    mov si, [bp+4] ; src 주소
    mov di, [bp+6] ; dst 주소
    xor bx, bx
    add di, MSG_LENGTH

rloop:
    mov al, BYTE PTR [si+bx]
    dec di
    
    mov BYTE PTR [di], al
    inc bx
    
    mov al, BYTE PTR [si+bx]
    cmp al, '$'
    jne rloop

    pop bp
    ret

rvs ENDP

END


