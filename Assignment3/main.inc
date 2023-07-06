.DATA
prompt1 DB "=== Type File Name ===",13,10,'$'
prompt2 DB "=== Type follow sentence ===",13,10,'$'
astMsg DB "YOU MISSED TYPED!",13,10,'$'

BUFFER_SIZE EQU 255
BUFFER_START EQU buffer+2
buffer DB BUFFER_SIZE, 0, BUFFER_SIZE DUP ('$'), '$'
FILENAME_SIZE EQU 12
filename DB FILENAME_SIZE, 0, FILENAME_SIZE DUP (?), '$'
FILENAME_START EQU filename+2
newline DB 0Dh, 0Ah, '$'
zero DW 0, 0

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

    mov cx, ds
    mov ds, zero
    mov bx, 0ch
    mov ax, [bx]
    mov dx, [bx+2]
    mov ds, cx
    mov WORD PTR keep_orgint3, ax
    mov WORD PTR keep_orgint3+2, dx

    mov ax, OFFSET int3_handler
    mov ds, zero
    mov [bx], ax
    mov [bx+2], cs
    mov ds, cx
    
    ;mov ax, 3503h
    ;int 21h
    ;mov WORD PTR keep_orgint3, bx
    ;mov WORD PTR keep_orgint3+2, es
    
    ;mov bx, ds

    ;mov ax, cs
    ;mov ds, ax
    ;mov dx, OFFSET int3_handler
    ;mov ax, 2503h
    ;int 21h

    ;mov ds, bx

    print prompt1

getFileName:
    mov ah, 0Ah            
    mov dx, OFFSET filename
    int 21h
    
    print newline    ; 받아온 이름에 뉴라인만 출력함

    mov bl, filename[1]
    xor bh, bh
    mov BYTE PTR [bx+FILENAME_START], 00h

    print prompt2

openFile:
    mov ax, 3D00h    ; 파일 열기
    mov dx, OFFSET FILENAME_START
    int 21h

    push ax    ; file handle 보관

readFile:
    lea si, BUFFER_START
    mov bx, ax                    ; 파일 내용 읽어오기
    mov cx, 255
    mov dx, si
    mov ah, 3Fh
    int 21h

    mov bx, ax                    ; 실제 읽어온 길이가 ax에
    mov cx, ax                    ; 읽어온 문자열 길이를 cx에
    jcxz exitGetC

    mov BYTE PTR [bx+si], 0Dh
    mov BYTE PTR [bx+si+1], 0Ah
    
    print BUFFER_START            ; 출력하기

preLoopGetC:
    xor bh, bh

loopGetC:
    mov ah, 01h
    int 21h
    cmp al, 0Dh
    jz exitGetC

    xor ah, ah
    mov bl, [si]
    
    assert bx, ax, astMsg

    inc si
    loop loopGetC

exitGetC:
    print newline

rcvInt3:
   
    lds dx, keep_orgint3
    mov ax, 2503h
    int 21h

closeFile:
    pop bx
    mov ah, 3eh
    int 21h

exitProgram:

    mov ax, 4c00h
    int 21h
