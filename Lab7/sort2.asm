TITLE sort2

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA
msg DB 0, 0, 255 DUP(0)
PADDING EQU 2
msg_length DW ?
MSG_START EQU msg+2
pivot DW ?
indexI DW ?

.CODE
.STARTUP

    lea bx, msg
    push bx
    call read_string
    add sp, 2

    mov al, msg+1
    xor ah, ah
    mov msg_length, ax

    mov [bx+msg_length], '$'
    
quickSort:              ; 간접피연산자로 사용하는 레지스터는 bx, bp, si, di
    xor si, si          ; si를 left로, di를 right로
    mov di, msg_length
    dec di              ; di를 마지막 인덱스로 셋팅
    call recursive
    

recursive:
    
    cmp si, di
    jnb efunc

    push di
    push si
    call getpivot       ; pivot을 담아옴
    
    pop si

    mov di, pivot
    dec di
    call recursive

    mov si, pivot
    inc si
    pop di
    call recursive

getpivot:
    mov indexI, si
    dec indexI

ploop:
    inc indexI
    cmp indexI, di
    jnb exitPvt
    mov al, [bx+indexI]
    cmp al, [bx+di]
    jae ploop
    
    ; swap
    mov ah, [bx+si]
    xchg al, ah
    mov [bx+indexI], al
    mov [bx+si], ah
    inc si
    jmp ploop


exitPvt:
    mov pivot, si

    ; swap num[left], num[right] 
    mov ah, [bx+si]
    mov al, [bx+di]
    xchg al, ah
    mov [bx+si], ah
    mov [bx+di], al

    ret


efunc:

    ret



exit:
    mov ah, 09h
    lea dx, MSG_START
    int 21h

    mov ah, 4ch
    xor al, al
    int 21h

INCLUDE readstr.inc

END