#include "canvas.h"
#include "palette.h"
#include <stdio.h>

#define MAX_SIZE (32)
#define TRUE (1)
#define FALSE (0)

static canvas_t s_canvas;

void set_vector_by_opcode_xy(unsigned char xy);

void set_canvas(unsigned char* canvas32x32)
{
    s_canvas.m_canvas = canvas32x32;
    
    if (s_canvas.p_palette == NULL) {
        s_canvas.p_palette = get_palette(0);
    }
    if (s_canvas.p_brush == NULL) {
        s_canvas.p_brush = get_palette(0);
    }
    
    return;
}

void execute(unsigned char instruction)
{
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

        p_color = s_canvas.p_palette + operand;

        for (i = 0; i < MAX_SIZE; ++i) {
            for (j = 0; j < MAX_SIZE; ++j) {
                s_canvas.m_canvas[j * MAX_SIZE + i] = *p_color;
            }
        }
        break;
    case OPCODE_SET_PALETTE:
        s_canvas.p_palette = get_palette(operand) + s_canvas.color_idx;
        break;
    case OPCODE_SET_X:
        s_canvas.x_pos = operand;
        break;
    case OPCODE_SET_Y:
        s_canvas.y_pos = operand;
        break;
    case OPCODE_DRAW_COLOR:
        p_color = s_canvas.p_palette + operand;
        s_canvas.m_canvas[s_canvas.y_pos * MAX_SIZE + s_canvas.x_pos] = *p_color;
        break;
    case OPCODE_SET_BRUSH:
        s_canvas.color_idx = operand;
        s_canvas.p_brush = s_canvas.p_palette[0] + s_canvas.color_idx;
        break;
    case OPCODE_SET_XY:
        set_vector_by_opcode_xy(operand);
        break;
    case OPCODE_SET_MOV:
        
        pendown = (operand & 0b10000) >> 4;
        x_dir = (operand & 0b01100) >> 2;
        y_dir = operand & 0b011;
        
        if (pendown == TRUE) {
            s_canvas.m_canvas[s_canvas.y_pos * MAX_SIZE + s_canvas.x_pos] = *(s_canvas.p_brush);
        }

        if (x_dir > 0) {
            s_canvas.x_pos += x_dir % 2 == 1 ? 1 : -1;
            s_canvas.x_pos = s_canvas.x_pos == 0xff ? 31 : s_canvas.x_pos;
            s_canvas.x_pos = s_canvas.x_pos == 32 ? 0 : s_canvas.x_pos;
        }
        if (y_dir > 0) {
            s_canvas.y_pos += y_dir % 2 == 1 ? -1 : 1;
            s_canvas.y_pos = s_canvas.y_pos == 0xff ? 31 : s_canvas.y_pos;
            s_canvas.y_pos = s_canvas.y_pos == 32 ? 0 : s_canvas.y_pos;
        }
        if (pendown == TRUE) {
            s_canvas.m_canvas[s_canvas.y_pos * MAX_SIZE + s_canvas.x_pos] = *(s_canvas.p_brush);
        }

        break;
    default:
        printf("INVALID OPCODE!\n");
        break;
    }
}
void set_vector_by_opcode_xy(unsigned char xy)
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

    s_canvas.x_pos = x;
    s_canvas.y_pos = y;
}