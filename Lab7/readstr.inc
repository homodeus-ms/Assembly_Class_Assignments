read_string PROC NEAR
    push bp
    mov bp, sp
    mov bx, [bp+4]
    mov BYTE PTR [bx+1], 1
    mov BYTE PTR [bx+2], 'z'
    ;mov BYTE PTR [bx+3], 'a'
    
    pop bp
    ret
read_string ENDP