.DATA
prompt1 DB "=== Type File Name ===",13,10,'$'
prompt2 DB "=== Type follow sentence ===",13,10,'$'
astMsg DB "YOU MISSED TYPED!",13,10,'$'

BUFFER_SIZE EQU 255
BUFFER_START EQU buffer+2
buffer DB BUFFER_SIZE, 0, BUFFER_SIZE DUP (0), '$'
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
    
    mov ax, 3503h
    int 21h
    mov WORD PTR keep_orgint3, bx
    mov WORD PTR keep_orgint3+2, es

    mov bx, ds

    mov ax, cs
    mov ds, ax
    mov dx, OFFSET int3_handler
    mov ax, 2503h
    int 21h

    mov ds, bx

    print prompt1

getFileName:
    mov ah, 0ah            
    mov dx, OFFSET buffer
    int 21h
    
    print newline    ; 받아온 이름에 뉴라인만 출력함

    ;mov bl, buffer[1]
    ;xor bh, bh
    ;mov [bx+BUFFER_START], 00h

    print prompt2

openFile:
    mov ax, 3D00h    ; 파일 열기
    mov dx, OFFSET BUFFER_START
    int 21h

    ;push ax    ; file handle 보관
    mov bx, ax    ; file handle을 bx에 보관 계속 사용

readFile:
    ;mov bx, ax                    ; 파일 내용 읽어오기
    mov cx, 255
    mov dx, OFFSET BUFFER_START
    mov ah, 3Fh
    int 21h

    ;mov bx, ax                    ; 실제 읽어온 길이가 ax에 있음
    mov cx, ax                    ; 읽어온 문자열 길이를 cx에
    mov di, ax
    mov [di+BUFFER_START], '$'    ; 읽어온 문자열 마지막에 '$' 붙이고
    print BUFFER_START            ; 출력하기
    print newline

preLoopGetC:
    mov ah, 01h
    mov si, 0h        ; si = 인덱스
    
    mov dh, 01h      ; 밑에서 읽어온문자를 dx에, 원소스는 bx에
    ;xor bh, bh       ; => change, 읽어온것 ax, 원소스 dl, dh도 01h로
    ;mov di, OFFSET astMsg

loopGetC:
    int 21h
    cmp al, 0dh
    je exitGetC

    ;mov dl, al
    mov dl, [BUFFER_START+si]
    
    assert dx, ax, astMsg

    inc si
    loop loopGetC

exitGetC:
    
    print newline

rcvInt3:
   
    lds dx, keep_orgint3
    mov ax, 2503h
    int 21h

closeFile:
    mov ah, 3eh
    int 21h

exitProgram:

    mov ax, 4c00h
    int 21h
