
read_string PROC NEAR
    push bp
    mov bp, sp

    mov dx, [bp+4]
    mov ah, 0Ah
    int 21h

    pop bp
    ret
read_string ENDP