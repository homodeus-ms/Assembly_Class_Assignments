#include "canvas.h"
#include "palette.h"

#include <stdio.h>

#define MAX_SIZE (32)
#define TRUE (1)
#define FALSE (0)

static unsigned char* s_canvas = NULL;

void set_vector_by_opcode_xy(unsigned char xy, signed char* x_pos, signed char* y_pos);

void set_canvas(unsigned char* canvas32x32)
{
    s_canvas = canvas32x32;
    
    return;
}

void execute(unsigned char instruction)
{
    static signed char s_x_pos = 0;
    static signed char s_y_pos = 0;
    static const unsigned char* s_p_brush;
    static unsigned char s_palette_id = 0;

    opcode_t opcode = instruction >> 5;
    unsigned char operand = instruction & 0b00011111;
    unsigned char i = 0;
    unsigned char j = 0;
    const unsigned char* p_color;
    unsigned char pendown;
    unsigned char x_dir;
    unsigned char y_dir;
    
    switch (opcode) {
    case OPCODE_CLEAR:

        p_color = get_palette(s_palette_id) + operand;

        for (i = 0; i < MAX_SIZE; ++i) {
            for (j = 0; j < MAX_SIZE; ++j) {
                s_canvas[j * MAX_SIZE + i] = *p_color;
            }
        }
        break;
    case OPCODE_SET_PALETTE:
        s_palette_id = operand;
        break;
    case OPCODE_SET_X:
        s_x_pos = operand;
        break;
    case OPCODE_SET_Y:
        s_y_pos = operand;
        break;
    case OPCODE_DRAW_COLOR:
        p_color = get_palette(s_palette_id) + operand;
        s_canvas[s_y_pos * MAX_SIZE + s_x_pos] = *p_color;
        break;
    case OPCODE_SET_BRUSH:
        s_p_brush = get_palette(s_palette_id) + operand;
        break;
    case OPCODE_SET_XY:
        set_vector_by_opcode_xy(operand, &s_x_pos, &s_y_pos);
        break;
    case OPCODE_SET_MOV:
        pendown = (operand & 0b10000) >> 4;
        x_dir = (operand & 0b01100) >> 2;
        y_dir = operand & 0b011;
        
        if (pendown == TRUE) {
            s_canvas[s_y_pos * MAX_SIZE + s_x_pos] = *(s_p_brush);
        }

        if (x_dir > 0) {
            s_x_pos += x_dir % 2 == 1 ? 1 : -1;
            s_x_pos = s_x_pos < 0 ? 31 : s_x_pos;
            s_x_pos = s_x_pos == 32 ? 0 : s_x_pos;
        }
        if (y_dir > 0) {
            s_y_pos += y_dir % 2 == 1 ? -1 : 1;
            s_y_pos = s_y_pos < 0 ? 31 : s_y_pos;
            s_y_pos = s_y_pos == 32 ? 0 : s_y_pos;
        }
        if (pendown == TRUE) {
            s_canvas[s_y_pos * MAX_SIZE + s_x_pos] = *(s_p_brush);
        }

        break;
    default:
        printf("INVALID OPCODE!\n");
        break;
    }

    printf("x_pos: %d ", s_x_pos);
    printf("y_pos: %d\n", s_y_pos);
}
void set_vector_by_opcode_xy(unsigned char xy, signed char* x_pos, signed char* y_pos)
{
    unsigned char x = 0;
    unsigned char y = 0;
    xy &= 0b01111;
    unsigned quad = (xy & 0b01100);
    unsigned corner = (xy & 0b011);

    y += corner % 2 == 1 ? 15 : 0;
    x += corner >= 2 ? 15 : 0;
    y += quad >= 8 ? 16 : 0;
    x += (quad == 0 || quad == 12) ? 16 : 0;

    *x_pos = x;
    *y_pos = y;
}
