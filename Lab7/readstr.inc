read_string PROC NEAR
    push bp
    mov bp, sp
    mov bx, [bp+4]
    mov BYTE PTR [bx+1], 4
    mov BYTE PTR [bx+2], 'z'        ; z c a A B Z c a
    mov BYTE PTR [bx+3], 'Z'
    mov BYTE PTR [bx+4], 'a'
    mov BYTE PTR [bx+5], 'c'
    mov BYTE PTR [bx+6], 'A'
    mov BYTE PTR [bx+7], 'Z'
    mov BYTE PTR [bx+8], 'C'
    mov BYTE PTR [bx+9], 'a'
    mov BYTE PTR [bx+10], 'b'
    mov BYTE PTR [bx+11], 'a'
    
    pop bp
    ret
read_string ENDP