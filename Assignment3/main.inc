.DATA
prompt1 DB "=== Type File Name ===",13,10,'$'
prompt2 DB "=== Type follow sentence ===",13,10,'$'
astMsg DB "YOU MISSED TYPED!",13,10,'$'

BUFFER_SIZE EQU 255
BUFFER_START EQU buffer+2
buffer DB BUFFER_SIZE, 0, BUFFER_SIZE DUP (?), '$'
newline DB 0Dh, 0Ah, '$'

keep_orgint3 DD ?

.CODE
.STARTUP

; 매크로
print MACRO m
    lea dx, m
    mov ah, 09h
    int 21h
ENDM


; 코드 시작

chgInt3:
    
    cli

    mov al, 3
    mov ah, 35h
    int 21h
    mov WORD PTR keep_orgint3, bx
    mov WORD PTR keep_orgint3+2, es

    push ds

    mov ax, cs
    mov ds, ax
    mov dx, OFFSET int3_handler
    mov ax, 2503h
    int 21h

    pop ds

    sti

    print prompt1

getFileName:
    mov ah, 0ah            
    mov dx, OFFSET buffer
    int 21h

    ;print BUFFER_START     ; 받아온 파일이름 출력
    print newline

    mov bl, buffer[1]
    xor bh, bh
    mov [bx+BUFFER_START], 00h

    print prompt2

openFile:
    mov ah, 3dh            ; 파일 열기
    mov al, 00h
    mov dx, OFFSET BUFFER_START
    int 21h

    push ax

readFile:
    mov bx, ax              ; 파일 내용 읽어오기
    mov cx, 255
    mov dx, OFFSET BUFFER_START
    mov ah, 3fh
    int 21h

    mov bx, ax
    mov [bx+BUFFER_START], '$'    ; 읽어온 문자열 마지막에 '$' 붙이고
    print BUFFER_START            ; 출력하기
    print newline

preLoopGetC:
    mov ah, 01h
    
    mov si, 0        ; bx = 읽어온 문자열의 길이, si = 인덱스
    mov dh, 0        ; 밑에서 dl에 al을 넣고 dx를 assert로 보낼것임
    mov di, OFFSET astMsg

loopGetC:
    int 21h
    cmp al, 0dh
    je exitGetC

    mov dl, al        ; dx에 읽어온 글자
    mov cl, [BUFFER_START+si]
    xor ch, ch
    
    assert dx, cx, astMsg

    inc si
    cmp si, bx
    jne loopGetC
    

exitGetC:
    ;mov [BUFFER_START+di], '$'
    ;print BUFFER_START
    print newline


rcvInt3:
    cli
    lds dx, keep_orgint3
    mov ax, 2503h
    int 21h
    sti

closeFile:
    pop bx
    mov ah, 3eh
    int 21h

exitProgram:

    mov ah, 4ch
    xor al, al
    int 21h
