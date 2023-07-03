int3_handler PROC
    sti
    push ax
    push dx

    mov dx, di
    mov ah, 09h
    int 21h

    call keep_orgint3

    pop dx
    pop ax

    iret

int3_handler ENDP
