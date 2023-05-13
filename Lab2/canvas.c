#include "canvas.h"
#include "palette.h"

#include <stdio.h>

#define MAX_SIZE (32)
#define TRUE (1)
#define FALSE (0)

static canvas_t s_canvas = { 0, };

void set_vector_by_opcode_xy(unsigned char xy, signed char* x_pos, signed char* y_pos);

void set_canvas(unsigned char* canvas32x32)
{
    s_canvas.canvas = canvas32x32;
    s_canvas.p_brush = get_palette(s_canvas.palette_id);
    
    return;
}

void execute(unsigned char instruction)
{
    static signed char x_pos;
    static signed char y_pos;

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

        p_color = get_palette(s_canvas.palette_id) + operand;

        for (i = 0; i < MAX_SIZE; ++i) {
            for (j = 0; j < MAX_SIZE; ++j) {
                s_canvas.canvas[j * MAX_SIZE + i] = *p_color;
            }
        }
        break;
    case OPCODE_SET_PALETTE:
        s_canvas.palette_id = operand;
        break;
    case OPCODE_SET_X:
        x_pos = operand;
        break;
    case OPCODE_SET_Y:
        y_pos = operand;
        break;
    case OPCODE_DRAW_COLOR:
        p_color = get_palette(s_canvas.palette_id) + operand;
        s_canvas.canvas[y_pos * MAX_SIZE + x_pos] = *p_color;
        break;
    case OPCODE_SET_BRUSH:
        s_canvas.p_brush = get_palette(s_canvas.palette_id) + operand;
        break;
    case OPCODE_SET_XY:
        set_vector_by_opcode_xy(operand, &x_pos, &y_pos);
        break;
    case OPCODE_SET_MOV:
        pendown = (operand & 0b10000) >> 4;
        x_dir = (operand & 0b01100) >> 2;
        y_dir = operand & 0b011;
        
        if (pendown == TRUE) {
            s_canvas.canvas[y_pos * MAX_SIZE + x_pos] = *(s_canvas.p_brush);
        }

        if (x_dir > 0) {
            x_pos += x_dir % 2 == 1 ? 1 : -1;
            x_pos = x_pos < 0 ? 31 : x_pos;
            x_pos = x_pos == 32 ? 0 : x_pos;
        }
        if (y_dir > 0) {
            y_pos += y_dir % 2 == 1 ? -1 : 1;
            y_pos = y_pos < 0 ? 31 : y_pos;
            y_pos = y_pos == 32 ? 0 : y_pos;
        }
        if (pendown == TRUE) {
            s_canvas.canvas[y_pos * MAX_SIZE + x_pos] = *(s_canvas.p_brush);
        }

        break;
    default:
        printf("INVALID OPCODE!\n");
        break;
    }
}
void set_vector_by_opcode_xy(unsigned char xy, signed char* x_pos, signed char* y_pos )
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
