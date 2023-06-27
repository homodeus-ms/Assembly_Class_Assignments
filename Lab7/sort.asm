TITLE Sort

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA

msg DB 256 DUP (0)
PADDING EQU 2
msg_length DW ?
msg_start EQU msg+2

.CODE
.STARTUP

    lea bx, msg
    push bx
    call read_string
    mov sp, 2

sort_start:

    mov al, msg[1]           ; msg_length에 실제 문자열의 길이를 저장
    
    mov BYTE PTR msg_length, al
    mov BYTE PTR msg_length[1], 0h

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
    mov si, msg_length
    mov msg[si+PADDING], '$' 

    mov ah, 09h
    lea dx, msg[2]
    int 21h
    

    mov ah, 4Ch
    xor al, al
    int 21h

INCLUDE readstr.inc

END