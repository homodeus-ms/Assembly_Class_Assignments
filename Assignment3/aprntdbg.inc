int3_handler PROC
    sti
    push ax
    push dx

    mov dx, di
    mov ah, 09h
    int 21h
    
    push ds

    lds dx, keep_orgint3
    mov ax, 2503h
    int 21h

    int 3h
    
    pop ds
    

    push ds

    mov ax, cs
    mov ds, ax
    mov dx, OFFSET int3_handler
    mov ax, 2503h
    int 21h

    pop ds

    pop dx
    pop ax

    iret

int3_handler ENDP
