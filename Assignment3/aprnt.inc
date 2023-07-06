int3_handler PROC
    sti

    mov dx, di
    mov ah, 09h
    int 21h

    iret

int3_handler ENDP
