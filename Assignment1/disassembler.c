#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "disassembler.h"

#define ARR_SIZE (16)
#define MNEM_LENGTH (3)
#define MODE_MAX_LENGTH (7)

#define PRINT(hex, mnemo, mode, h, l) sprintf(out_buffer64, "OPCODE=%02X[%s %s] OPERAND=%s %s", hex, mnemo, mode, h, l)

static const unsigned char* s_mnemonics[ARR_SIZE][ARR_SIZE] = {
    { "brk i", "ora (zp,x)", "", "", "", "ora zp", "asl zp", "", "php i", "ora #", "asl A", "", "", "ora a", "asl a", "" },
    { "bpl r", "ora (zp),y", "", "", "", "ora zp,x", "asl zp,x", "", "clc i", "ora a,y", "", "", "", "ora a,x", "asl a,x", "" },
    { "jsr a", "and (zp,x)", "", "", "bit zp", "and zp", "rol zp", "", "plp i", "and #", "rol A", "", "bit a", "and a", "rol a", "" },
    { "bmi r", "and (zp),y", "", "", "", "and zp,x", "rol zp,x", "", "sec i", "and a,y", "", "", "", "and a,x", "rol a,x", "" },
    { "rti i", "eor (zp,x)", "", "", "", "eor zp", "lsr zp", "", "pha i", "eor #", "lsr A", "", "jmp a", "eor a", "lsr a", "" },
    { "bvc r", "eor (zp),y", "", "", "", "eor zp,x", "lsr zp,x", "", "cli i", "eor a,y", "", "", "", "eor a,x", "lsr a,x", "" },
    { "rts i", "adc (zp,x)", "", "", "", "adc zp", "ror zp", "", "pla i", "adc #", "ror A", "", "jmp (a)", "adc a", "ror a", "" },
    { "bvs r", "adc (zp),y", "", "", "", "adc zp,x", "ror zp,x", "", "sei i", "adc a,y", "", "", "", "adc a,x", "ror a,x", "" },
    { "", "sta (zp,x)", "", "", "sty zp", "sta zp", "stx zp", "", "dey i", "", "txa i", "", "sty a", "sta a", "stx a", "" },
    { "bcc r", "sta (zp),y", "", "", "sty zp,x", "sta zp,x", "stx zp,y", "", "tya i", "sta a,y", "txs i", "", "", "sta a,x", "", "" },
    { "ldy #", "lda (zp,x)", "ldx #", "", "ldy zp", "lda zp", "ldx zp", "", "tay i", "lda #", "tax i", "", "ldy a", "lda a", "ldx a", "" },
    { "bcs r", "lda (zp),y", "", "", "ldy zp,x", "lda zp,x", "ldx zp,y", "", "clv i", "lda a,y", "tsx i", "", "ldy a,x", "lda a,x", "ldx a,y", "" },
    { "cpy #", "cmp (zp,x)", "", "", "cpy zp", "cmp zp", "dec zp", "", "iny i", "cmp #", "dex i", "", "cpy a", "cmp a", "dec a", "" },
    { "bne r", "cmp (zp),y", "", "", "", "cmp zp,x", "dec zp,x", "", "cld i", "cmp a,y", "", "", "", "cmp a,x", "dec a,x", "" },
    { "cpx #", "sbc (zp,x)", "", "", "cpx zp", "sbc zp", "inc zp", "", "inx i", "sbc #", "nop i", "", "cpx a", "sbc a", "inc a", "" },
    { "beq r", "sbc (zp),y", "", "", "", "sbc zp,x", "inc zp,x", "", "sed i", "sbc a,y", "", "", "", "sbc a,x", "inc a,x", "" },
};

const unsigned char* disassemble(char* out_buffer64, const unsigned char* mem)
{
    assert(mem != NULL);

    const unsigned char* result;
    const unsigned char* p = mem;
    unsigned char read_hex;
    const unsigned char* opcode;
    unsigned char mnemonic[4];
    
    unsigned char index_x;
    unsigned char index_y;
   
    const unsigned char mode[MODE_MAX_LENGTH];
    signed short addr_l = -1;
    signed short addr_h = -1;

    read_hex = *mem;
    index_x = read_hex & 0b00001111;
    index_y = (read_hex & 0b11110000) >> 4;

    assert(index_x <= 0xf && index_y <= 0xf);

    opcode = s_mnemonics[index_y][index_x];

    if (opcode == "") {
        result = mem;
        return result;
    }

    mnemonic[0] = opcode[0];
    mnemonic[1] = opcode[1];
    mnemonic[2] = opcode[2];
    mnemonic[3] = '\0';
    
    strcpy(mode, opcode + 4);

    if (mode[0] == 'A' || mode[0] == 'i') {
        result = p + 1;
    } else if (mode[0] == 'a' || mode[1] == 'a') {
        addr_l = *(p + 1);
        addr_h = *(p + 2);
        result = p + 3;
    } else {
        addr_l = *(p + 1);
        result = p + 2;
    }

    char str_low_b[3] = { '.', '.', '\0' };
    char str_high_b[3] = { '.', '.', '\0' };

    if (addr_l != -1) {
        sprintf(str_low_b, "%02X", addr_l);
    }
    if (addr_h != -1) {
        sprintf(str_high_b, "%02X", addr_h);
    }
    
    PRINT(read_hex, mnemonic, mode, str_high_b, str_low_b);
    
    assert(result != NULL);

    return result;
}
