#include "canvas.h"
#include "palette.h"

#include <stdio.h>

#define MAX_SIZE (32)
#define TRUE (1)
#define FALSE (0)

static canvas_t s_canvas;

unsigned char* g_palette;

void set_vector_by_opcode_xy(unsigned char xy);

void set_canvas(unsigned char* canvas32x32)
{
    s_canvas.canvas = canvas32x32;
    s_canvas.x_pos = 0;
    s_canvas.y_pos = 0;

    return;
}

void execute(unsigned char instruction)
{
    opcode_t opcode = instruction >> 5;
    unsigned char operand = instruction & 0b00011111;
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char* color;
    unsigned char pendown;
    unsigned char dirX;
    unsigned char dirY;

    switch (opcode) {
    case OPCODE_CLEAR:

        color = g_palette + operand;

        for (i = 0; i < MAX_SIZE; ++i) {
            for (j = 0; j < MAX_SIZE; ++j) {
                s_canvas.canvas[j * MAX_SIZE + i] = *color;
            }
        }
        break;
    case OPCODE_SET_PALETTE:
        break;
    case OPCODE_SET_X:
        s_canvas.x_pos = operand;
        break;
    case OPCODE_SET_Y:
        s_canvas.y_pos = operand;
        break;
    case OPCODE_DRAW_COLOR:
        color = g_palette + operand;
        s_canvas.canvas[s_canvas.y_pos * MAX_SIZE + s_canvas.x_pos] = *color;
        break;
    case OPCODE_SET_BRUSH:
        s_canvas.brush_color = *(g_palette + operand);
        break;
    case OPCODE_SET_XY:
        set_vector_by_opcode_xy(operand);
        break;
    case OPCODE_SET_MOV:
        pendown = (operand & 0b10000) >> 4;
        dirX = (operand & 0b01100) >> 2;
        dirY = operand & 0b011;

        if (pendown == TRUE)
        {
            s_canvas.canvas[s_canvas.y_pos * MAX_SIZE + s_canvas.x_pos] = s_canvas.brush_color;
        }

        if (dirX > 0)
        {
            s_canvas.x_pos += dirX % 2 == 1 ? 1 : -1;
            s_canvas.x_pos = s_canvas.x_pos < 0 ? 31 : s_canvas.x_pos;
            s_canvas.x_pos = s_canvas.x_pos == 32 ? 0 : s_canvas.x_pos;
        }
        if (dirY > 0)
        {
            s_canvas.y_pos += dirY % 2 == 1 ? -1 : 1;
            s_canvas.y_pos = s_canvas.y_pos < 0 ? 31 : s_canvas.y_pos;
            s_canvas.y_pos = s_canvas.y_pos == 32 ? 0 : s_canvas.y_pos;
        }
        if (pendown == TRUE)
        {
            s_canvas.canvas[s_canvas.y_pos * MAX_SIZE + s_canvas.x_pos] = s_canvas.brush_color;
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
    x += (quad == 0 || quad == 9) ? 16 : 0;

    s_canvas.x_pos = x;
    s_canvas.y_pos = y;
}
