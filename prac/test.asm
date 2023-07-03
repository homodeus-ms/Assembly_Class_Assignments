TITLE test

.DOSSEG
.8086
.NO87
.MODEL small

.STACK 100h

.DATA
msg DB "=== Write something ===$"
newline DB 10, 13, '$'
buffer DB 50 DUP (?)


.CODE
.STARTUP
    
print MACRO m
    lea dx, m
    mov ah, 09h
    int 21h
ENDM

    mov ax, 5
    mov bx, 3
    mov ax, OFFSET sum
    lea bx, sum
    mov ax, 7
    mov ax, 9


sum PROC
    mov ax, 5
    add ax, 3

    ret
sum ENDP



    ; 프로그램 종료
    mov ah, 4Ch
    int 21h

END