TITLE Sort

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA

MAX_LENGTH EQU 254
msg DB MAX_LENGTH, 0, 255 DUP (MAX_LENGTH)
PADDING EQU 2
msg_length DW ?
msg_start EQU msg+2

.CODE
.STARTUP

    lea bx, msg
    push bx
    call read_string
    add sp, 2

    mov al, msg[1]
    xor ah, ah

    mov si, ax
    mov msg_length, ax
    mov msg[si+PADDING], '$'

sort_start:

    cmp al, 1
    jbe exit                 ; 길이가 1 이하면 바로 exit

    mov bx, msg_length 
    mov bp, msg_length
    dec bx           ; 전체길이 - 1
    
    mov si, 0FFFFH  

loop1:               ; si = i / bx = i종료조건 / di = j / bp = j종료조건
    inc si
    cmp si, bx
    jnb exit
    mov di, 0FFFFH
    dec bp

loop2:
    inc di
    cmp di, bp
    jnb loop1

    mov al, msg_start[di]
    cmp al, msg_start[di+1]
    jna loop2

    mov ah, msg_start[di+1]
    xchg al, ah
    mov msg_start[di], al
    mov msg_start[di+1], ah

    jmp loop2

exit:
    
    mov ah, 09h
    lea dx, msg[2]
    int 21h
    

    mov ah, 4Ch
    xor al, al
    int 21h

INCLUDE readstr.inc

END