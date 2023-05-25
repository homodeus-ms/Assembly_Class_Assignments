STR_FLAG_N=$2300
STR_FLAG_V=$2301
STR_FLAG_Z=$2302
STR_FLAG_C=$2303

MASK_N=$80
MASK_V=$40
MASK_Z=$02
MASK_C=$01


    php
    pla
    tax    ; 9 cycle

    and #MASK_N
    clc
    rol
    rol
    sta STR_FLAG_N    ; 13 cycle

    txa
    and #MASK_V
    clc
    rol
    rol
    rol
    sta STR_FLAG_V    ; 17 cycle

    txa
    and #MASK_Z
    lsr
    sta STR_FLAG_Z    ; 10 cycle

    txa
    and #MASK_C
    sta STR_FLAG_C    ; 8 cycle

    