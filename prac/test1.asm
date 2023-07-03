TITLE read file and use console

.DOSSEG
.8086
.NO87
.MODEL SMALL

.STACK 100h

.DATA

filename DB "text.txt", 0
prompt1 DB "=== Type file name ===",13,10,'$' 
prompt2 DB "=== Type showed message ===",13,10,'$'
BUFFER_SIZE EQU 255
buffer DB BUFFER_SIZE, 0, BUFFER_SIZE DUP (?), 13, 10, '$'
BUFFER_START EQU buffer+2
newline DB 13, 10, '$'

.CODE
.STARTUP

print MACRO m
    lea dx, m
    mov ah, 09h
    int 21h
ENDM

add$ATE MACRO n
    mov bl, n
    xor bh, bh
    mov [bx+BUFFER_START], '$'
    mov [bx+BUFFER_START+1], '0'
ENDM



    print prompt1

    mov ah, 0ah    ; 파일 이름 받아오기
    lea dx, buffer
    int 21h

    add$ATE buffer+1

    print BUFFER_START    ; 파일 이름 출력
    print newline

    print prompt2

    mov ah, 3dh      ; 파일열기
    mov al, 00100000b
    mov dx, OFFSET BUFFER_START
    int 21h

    push ax

    mov bx, ax    ; 읽어오기
    mov cx, 255
    mov dx, OFFSET BUFFER_START
    mov ah, 3fh
    int 21h

    mov bx, ax
    mov [bx+BUFFER_START], '$'

    print BUFFER_START    ; 파일 내용 출력해서 보여주기
    print newline


    mov ah, 01h
    mov si, 2
inloop:
    int 21h
    cmp al, 0dh
    je exit
    mov [buffer+si], al
    inc si
    jmp inloop

exit:
    mov [buffer+si], '$'

    print BUFFER_START
    print newline

    pop bx    ; 파일닫기
    mov ah, 3eh
    int 21h

    mov ah, 4ch
    xor al, al
    int 21h

END



