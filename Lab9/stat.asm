TITLE Stat Printer

.DOSSEG
.8086
.8087
.MODEL TINY

.DATA
MAX_NAME EQU 12
fileName DB MAX_NAME, 0, MAX_NAME DUP ('$'), '$'
prompt DB "fileName?", 0Dh, 0Ah, '$'
secUnit DB ?
readNum DQ ?
average DW 0
readCnt DW ?
thousand DW 1000
ten DW 0Ah

res0 DB 0, 0, 0, '.'
res1 DB 0, 0, 0, '$'

newLine DB 0Dh, 0Ah, '$'


.CODE
.STARTUP

print MACRO m
    mov ah, 09h
    lea dx, m
    int 21h
ENDM

    finit

    print prompt

    ; 파일이름 읽기
    mov ah, 0Ah
    lea dx, fileName
    int 21h

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
    mov ah, 3Fh
    lea dx, readNum
    xor si, si

    fldz
    fwait

readAddLoop:
    mov ah, 3Fh
    int 21h
    cmp ax, 8
    jne saveResult

    fadd readNum
    fwait
    inc si
    jmp readAddLoop

saveResult:
    mov cl, secUnit       ; cl에 m or u 저장
    mov readCnt, si
    fidiv readCnt         ; get Average
    fwait

    cmp cl, 'm'
    je saveAverage

getMicro:
    fidiv thousand

saveAverage:
    frndint
    fistp average
    fwait

    xor dx, dx
    mov ax, average
    mov bx, OFFSET res1 - 1
    mov di, OFFSET saveIntPart

setSi3:
    mov si, 3
    jmp printSum
setSi2:
    mov si, 2


printSum:
    xor dx, dx
    div ten
    add dx, '0'
    mov [bx+si], dl
    
    dec si
    jnz printSum
    jmp di
    
saveIntPart:
    
    mov di, OFFSET Exit0
    lea bx, res0
    cmp cl, 'm'
    je setSi2
    dec bx
    jmp setSi3

Exit0:
    cmp cl, 'm'
    jne printMicro

printMilli:
    print res0 + 1
    jmp closeFile
    
printMicro:
    cmp res0, '0'
    je printMilli
    print res0

closeFile:
    pop bx
    mov ah, 3Eh
    int 21h

    mov ah, 4ch
    xor al, al
    int 21h

END