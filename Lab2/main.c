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

void print_canvas(unsigned char canvas[])
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            printf("%#x ", canvas[j * MAX_SIZE + i]);
        }
        puts("");
    }
    puts("");
}

int main(void)
{
    unsigned char canvas[MAX_SIZE * MAX_SIZE];

    set_canvas(canvas);

    execute(GET_INSTRUCTION(OPCODE_CLEAR, 1));
    print_canvas(canvas);
    execute(GET_INSTRUCTION(OPCODE_SET_MOV, 0b10110));
    print_canvas(canvas);
    execute(GET_INSTRUCTION(OPCODE_SET_BRUSH, 2));
    execute(GET_INSTRUCTION(OPCODE_SET_MOV, 0b10110));
    print_canvas(canvas);
    execute(GET_INSTRUCTION(OPCODE_SET_MOV, 0b10110));
    print_canvas(canvas);
    
    printf("No Assert\n");
}



