TITLE print reverse string

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA

msg DB "Hello Assembly!$" ; 0126

.CODE
.STARTUP

    mov bx, OFFSET msg
    push bx
    call reverse
    add sp, 2

    mov ah, 4ch
    xor al, al
    int 21h


reverse PROC
    ; push bp
    mov bp, sp
    mov bx, [bp+2]
    xor si, si
recursive PROC
    mov ax, [bx+si]
    cmp ax, '$'
    je efunc

    inc si
    call recursive
    

return:
    dec si
    mov dx, [bx+si]
    
    mov ah, 02h
    int 21h

efunc:
    ret

recursive ENDP

reverse ENDP

END