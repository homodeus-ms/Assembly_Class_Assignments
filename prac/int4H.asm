TITLE int4 handler

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA
msg DB "OVERFLOW!!$"
vector DD ?


.CODE
.STARTUP

    mov dx, ds

    mov bx, 0010h
    mov ax, 0
    mov ds, ax

    mov ax, [bx]
    mov WORD PTR vector, ax
    mov ax, [bx+2]
    mov WORD PTR vector+2, ax

    mov ax, OFFSET int4_handler
    mov bx, 10h
    mov [bx], ax
    mov [bx+2], dx

    
    mov ax, 07FFFh
    add ax, 2
    into
    

    mov ax, 0
    mov ds, ax

    mov ax, WORD PTR vector
    mov bx, 10h
    mov [bx], ax
    mov ax, WORD PTR vector+2
    mov [bx+2], ax

    mov ax, 4c00h
    int 21h

int4_handler PROC FAR
    ;sti

    mov ds, dx
    lea dx, msg
    mov ah, 09h
    int 21h
    
    iret

int4_handler ENDP

END
