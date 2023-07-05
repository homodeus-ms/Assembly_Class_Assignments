TITLE Formatted Print

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA
prompt1 DB "=== Type Formatted String ===", 13, 10, '$'
prompt2 DB "=== Type First Number ===", 13, 10, '$'
prompt3 DB "=== Type Second Number ===", 13, 10, '$'

MAX_LENGTH EQU 255
buffer DB MAX_LENGTH, 0, MAX_LENGTH DUP('$'), '$'
result DB MAX_LENGTH DUP ('$'), '$'

num1 DB ?
num2 DB ?
abs DB ?


.CODE
.STARTUP

INCLUDE macros.mac

    print prompt1    ; format 받기

    lea dx, buffer
    mov ah, 0Ah
    int 21h

    print prompt2    ; input1 받기

    readChar
    convertToInt num1

    print prompt3    ; input2 받기
    
    readchar
    convertToInt num2

    mov al, num1

    cmp al, num2
    jnb num1_Max
    mov bl, num2
    add bl, '0'
    mov dl, num1
    add dl, '0'      ; bl = max, dl = min
    
    xchg al, num2
    sub al, num2
    jmp getAbs


num1_Max:
    mov bl, num1
    add bl, '0'
    mov dl, num2
    add dl, '0'
    sub al, num2   ; max포인터에 max char, min포인터에 min char
    
    
getAbs:
    add al, '0'    ; abs안에 abs result char
    mov abs, al

getFormat:
    
    mov cl, BYTE PTR buffer+1
    mov si, OFFSET buffer+2
    mov di, OFFSET result

check_format:
    mov al, [si]
    cmp al, '%'
    jne write
    
    inc si
    mov al, [si]
    cmp al, 'm'
    je putmin
    cmp al, 'M'
    je putmax
    cmp al, 'd'
    je putabs
    dec si

write:
    mov al, BYTE PTR [si]
    mov BYTE PTR [di], al
continue:    
    inc si
    inc di
    loop check_format

    jmp printResult

putmin:
    mov [di], dl
    dec cx
    jmp continue
putmax:
    mov [di], bl
    dec cx
    jmp continue
putabs:
    mov al, abs
    mov [di], al
    dec cx
    jmp continue

printResult:
    print result
    
exit0:
    mov ah, 4ch
    xor al, al
    int 21h
END
