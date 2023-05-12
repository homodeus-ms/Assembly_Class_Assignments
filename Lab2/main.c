#include <assert.h>
#include <stdio.h>

#include "canvas.h"
#include "palette.h"

#define MAX_SIZE (32)

#define OPCODE_CLEAR        (0b000)
#define OPCODE_SET_PALETTE  (0b001)
#define OPCODE_SET_X        (0b010)
#define OPCODE_SET_Y        (0b011)
#define OPCODE_DRAW_COLOR   (0b100)
#define OPCODE_SET_BRUSH    (0b101)
#define OPCODE_SET_XY       (0b110)
#define OPCODE_MOV          (0b111)

#define GET_INSTRUCTION(opcode, operand) (((opcode) << 5) | (operand))

int main(void)
{
    unsigned char canvas[MAX_SIZE * MAX_SIZE];

    set_canvas(canvas);

    unsigned char palette_index = 0;
    unsigned char color_index = 20;

    const unsigned char* palette = get_palette(palette_index);
    unsigned char color = palette[color_index];

    execute(GET_INSTRUCTION(0b000, color_index));

    execute(0b11110110);

    execute(GET_INSTRUCTION(OPCODE_SET_BRUSH, 1));

    execute(0b11111001);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 31; ++j) {
            printf("%#x ", canvas[j * MAX_SIZE + i]);
        }
        printf("\n");
    }
    puts("");

    unsigned char canvas2[MAX_SIZE * MAX_SIZE];
    set_canvas(canvas2);
    palette_index = 1;
    color_index = 0;

    execute(GET_INSTRUCTION(OPCODE_SET_PALETTE, palette_index));

    palette = get_palette(palette_index);
    color = palette[color_index];

    execute(GET_INSTRUCTION(0b000, color_index));

    execute(0b11110110);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 31; ++j) {
            printf("%#x ", canvas2[j * MAX_SIZE + i]);
        }
        printf("\n");
    }
    puts("");

    printf("NoAssert!\n");

    return 0;
}
