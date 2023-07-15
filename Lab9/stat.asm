TITLE Stat Printer

.DOSSEG
.8086
.8087
.MODEL TINY

.DATA
MAX_NAME EQU 20
fileName DB MAX_NAME, 0, MAX_NAME DUP (?), '$'
prompt DB "Name?", 0Dh, 0Ah, '$'
secUnit DB ?
readNum DQ ?
average DW 0

average2 DD 0

readCnt DW ?
thousand DW 1000
onebytsnd DQ 3F50624DD2F1A9FCh
ten DW 0Ah

res0 DB '0', '0', '.', '0', '0', '0', '$'
newLine DB 0Dh, 0Ah, '$'

.CODE
.STARTUP

print MACRO m
    mov ah, 09h
    lea dx, m
    int 21h
ENDM

endprog MACRO
    mov ax, 4c00h
    int 21h
ENDM

    finit
    fldz
    fwait

    print prompt

    ; 파일이름 읽기
    mov ah, 0Ah
    lea dx, fileName
    int 21h

    xor bh, bh
    mov bl, BYTE PTR filename+1
    mov [bx+filename+2], '0'

    ; 파일 오픈
    mov ax, 3D00h
    mov dx, OFFSET fileName+2
    int 21h

    push ax
    mov bx, ax

    print newLine

    ; ===== 파일 읽어와서 연산 =====

    ; 첫 글자(단위) 빼기
    lea dx, secUnit
    mov cx, 1
    mov ah, 3Fh
    int 21h

    ; loop 시작
    mov cx, 8
    lea dx, readNum
    xor si, si

readAddLoop:
    mov ah, 3Fh
    int 21h
    cmp ax, 0
    je saveResult

    fld readNum
    fadd
    fwait
    inc si
    jmp readAddLoop

saveResult:

    mov cl, secUnit       ; cl에 m or u 저장
    mov readCnt, si
    
    fidiv readCnt        ; get Average
    fwait

    cmp cl, 'm'
    je saveAverage

getMicro:
    
    fld onebytsnd
    fmul
    fwait

saveAverage:
    
    fistp average2
    fwait

    mov ax, WORD PTR average2
    mov bx, OFFSET res0
    
convertLoop:

    xor dx, dx
    div ten
    add [bx+5], dl
    cmp ax, 0
    je printRes0
    
    xor dx, dx
    div ten
    add [bx+4], dl
    cmp ax, 0
    je printRes0

    xor dx, dx
    div ten
    add [bx+3], dl
    cmp ax, 0
    je printRes0

    xor dx, dx
    div ten
    add [bx+1], dl
    cmp ax, 0
    je printRes0

    xor dx, dx
    div ten
    add [bx], dl

printRes0:
    
    print res0

closeFile:
    pop bx
    mov ah, 3Eh
    int 21h

    endprog

END