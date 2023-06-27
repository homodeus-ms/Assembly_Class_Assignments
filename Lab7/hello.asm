TITLE Hello x86

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA
msg DB "Hello x86!$"

.CODE
.STARTUP
    ; 화면 출력
    mov ah, 09h
    lea dx, msg
    int 21h

    ; 프로그램 종료
    mov ah, 4ch
    xor al, al
    int 21h

END