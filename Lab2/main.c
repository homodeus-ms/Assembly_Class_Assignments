#include <assert.h>

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

    unsigned char* palette = get_palette(palette_index);
    unsigned char color = palette[color_index];

    execute(GET_INSTRUCTION(OPCODE_CLEAR, color_index));


#if 1
    printf("canvas color: %#x\n", canvas[5]);
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            assert(canvas[j * MAX_SIZE + i] == color);
        }
    }
    palette_index = 5;
    color_index = 1;
    palette = get_palette(palette_index);
    color = palette[color_index];
    execute(GET_INSTRUCTION(OPCODE_CLEAR, color_index));

    printf("canvas_color2: %#x\n", canvas[5]);

#endif

#if 1

    palette_index = 27;
    color_index = 19;

    palette = get_palette(palette_index);
    color = palette[color_index];

    unsigned char x = 7;
    unsigned char y = 12;
    
    execute(GET_INSTRUCTION(OPCODE_SET_PALETTE, palette_index));
    
    execute(GET_INSTRUCTION(OPCODE_SET_X, x));
    execute(GET_INSTRUCTION(OPCODE_SET_Y, y));

    execute(GET_INSTRUCTION(OPCODE_DRAW_COLOR, color_index));
    
    assert(canvas[y * MAX_SIZE + x] == color);
#endif

#if 1
    unsigned char xy = 0b01001;

    execute(GET_INSTRUCTION(OPCODE_SET_XY, xy));

    color_index = 3;
    color = palette[color_index];
    
    execute(GET_INSTRUCTION(OPCODE_DRAW_COLOR, color_index));

    x = 0;
    y = 31;
    
    assert(canvas[y * MAX_SIZE + x] == color);

#endif

#if 1
    color_index = 11;
    color = palette[color_index];

    execute(GET_INSTRUCTION(OPCODE_SET_BRUSH, color_index));
    execute(GET_INSTRUCTION(OPCODE_MOV, 0b10010));

    y = 0;
    printf("color: %#x\n", color);
    printf("y * MAX_SIZE + x : %#x\n", canvas[y * MAX_SIZE + x]);
    assert(canvas[y * MAX_SIZE + x] == color);

    execute(GET_INSTRUCTION(OPCODE_MOV, 0b00110));

    ++x;
    ++y;

    assert(canvas[y * MAX_SIZE + x] != color);
    printf("color: %#x\n", color);

    execute(GET_INSTRUCTION(OPCODE_MOV, 0b10110));

    assert(canvas[y * MAX_SIZE + x] == color);
    assert(canvas[(y + 1) * MAX_SIZE + (x + 1)] == color);

#endif

    printf("NoAssert!\n");

    return 0;
}
