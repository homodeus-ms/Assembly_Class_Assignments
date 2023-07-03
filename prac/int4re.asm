TITLE int4 handler

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA

msg DB "OVERFLOW!!$"
keep_int4 DD ?

.CODE
.STARTUP

    mov dx, ds
    mov ax, 0
    mov ds, ax
    mov bx, 10h

    push [bx]
    push [bx+2]

    mov ax, OFFSET int4_handler
    mov [bx], ax
    mov [bx+2], dx

    mov ax, 7fffH
    add ax, 2
    into

    pop [bx]
    pop [bx+2]

    mov ax, 4c00h
    int 21h

int4_handler PROC FAR
    push bp
    mov bp, sp

    mov ds, dx

    lea dx, msg
    mov ah, 09h
    int 21h

    pop bp

    iret
int4_handler ENDP

END

    

