TITLE 1ton2

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA
num DB 100

.CODE
.STARTUP

    mov al, num
    add ax, 1
    mul num
    shr ax, 1

    push ax
    call uitoa
    add sp, 2

    mov ax, 4c00h
    int 21h

INCLUDE uitoa.asm

END