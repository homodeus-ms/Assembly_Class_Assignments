TITLE Sort

.DOSSEG
.8086
.NO87
.MODEL TINY

.DATA
MSG_LENGTH EQU 254
msg DB MSG_LENGTH, 0, MSG_LENGTH DUP (?), '$'
MSG_START EQU msg+2

.CODE
.STARTUP

    lea ax, msg
    push ax
    call read_string

    ; 생략가능?
    add sp, 2
    lea ax, msg
    inc ax
    inc ax
    push ax
    
    call quickSort

    mov bl, msg+1
    mov msg[bx+2], '$'

    lea dx, MSG_START
    mov ah, 09h
    int 21h

    mov ah, 4Ch
    xor al, al
    int 21h

quickSort PROC NEAR    ; si left / di right / cx i
    push bp
    mov bp, sp
    mov bx, [bp+4]

    mov si, 0
    mov di, [bx-1]
    and di, 00ffh
    dec di  ; 라스트 인덱스

    push di
    push si
    push bx
    call recursive

    pop bp
    ret

recursive PROC NEAR    ; 재귀부분 (bx+2, left(si), right(di))
    push bp
    push si
    push di
    mov bp, sp

    mov bx, [bp+8]
    mov si, [bp+10]
    mov di, [bp+12]

    cmp si, di
    jae efunc

    push di
    push si
    push bx
    
    call getPivot

    add sp, 6

    mov di, dx
    dec di        ; pivot - 1
    push di
    push si
    push bx
    call recursive    ; 첫번째 재귀

    add sp, 6

    mov si, dx
    inc si
    push di
    push si
    push bx
    call recursive    ; 두번째 재귀


getPivot PROC NEAR    ; (msg+2, left(si), right(di)) -> pivot(dx)
                      ; cx가 left, si가 i 간접피연산자를 쓰려면 어쩔수없음
                      ; dx에 right 보관(스왑할때)
    push bp
    push si
    push di
    mov bp, sp
    mov bx, [bp+8]
    mov si, [bp+10]
    mov di, [bp+12]

    mov cx, si    ; cx가 left, 이제부터 si를 i로 쓸것
    mov si, 0FFFFH
pivot_loop:
    inc si
    cmp si, di
    jae exitloop

    mov al, BYTE PTR [bx+si]
    cmp al, BYTE PTR [bx+di]
    jae pivot_loop

    mov dx, di    ; 1. 여기서 스왑을 해야함 2. si, di둘다 필요함 3. di의 right값을 dx에 보관
    mov di, cx    ; 4. di에 cx값을 가져옴(left)
    xchg al, BYTE PTR [bx+di]    ; nums[i] 값과 nums[left] 값을 교환함
    mov BYTE PTR [bx+si], al

    mov di, dx    ; 다시 di에 right값 복구

    inc cx

    jmp pivot_loop

exitloop:

    mov dx, cx    ; 리턴값
    mov si, cx    ; left와 right의 값을 교환하기 위해 si에 cx대입

    mov al, BYTE PTR [bx+si]
    xchg al, BYTE PTR [bx+di]
    mov BYTE PTR [bx+si], al

    pop di
    pop si
    pop bp
    ret

getpivot ENDP

efunc:
    pop di
    pop si
    pop bp
    ret

recursive ENDP

quickSort ENDP

INCLUDE readstr.inc

END